#pragma once
#include <string_view>
#include <vector>
#include "Bin.h"
#include "Coordinate.h"
#include <valarray>
#include <utility>
#include <string>
#include <type_traits>
#include <execution>
#include <algorithm>
#include "Interval.h"
// FirstAnalyticParticleGraph.h: Creates a brute force particle graph with a given beta. 
// This class is the first 
// SingleChannelPoissonDistribution class generalized to N dimensions/channels,
// and uses a brute force method  
// which is much less smart (and therefore slower) than the SecondAnalyticParticleGraph class.
class FirstAnalyticParticleGraph 
{
	public:
		//Serves as a helper class to iterate over all of phase space
		class PhaseIterator
		{
			public:
				PhaseIterator(int dims);
				//Gets the current Coordinate which the PhaseIterator is on
				const Coordinate<int>& getCurrent();
				//Goes to the next Coordinate in the iterator.
				void next();
				//Checks if the iterator is exhausted.
				bool valid();

				//Makes the bins vector using PhaseIterator and ChannelReader 
				//(more info in FirstAnalyticParticleGraph constructor)
				template<typename ChannelReader>
				static void makeBins(std::vector<Bin>& bins, ChannelReader& channelReader, int dimensions, double beta);
			private:
				//Exponentiate an integer base to an integer exponent.
				static int powerInt(int base, int exponent);

				static constexpr int nUpper = 3000;
				Coordinate<int> currArray, endArray;
		};

		//Loads channels into a FirstAnalyticParticleGraph by using a ChannelReader template class
		//ChannelReader must have the subscript operator overloaded that returns a channel
		//Which has methods getMu and getObserved
		//As well as a method that gets the number of channels read called getChannelsRead().
		//For an implementation of this template, please see BasicChannelReader.h and
		//BasicChannel.h.
		template<typename ChannelReader>
		FirstAnalyticParticleGraph(ChannelReader& channelReader, double iBeta);

		//Check if the observed values are inside the confidence region
		bool checkConfidenceRegion(double confidenceLevel) const;
	private:
		int dimensions;
		double beta;
		double totalNormalizedLikelihood;
		Coordinate<int> observed;
		std::vector<Bin> bins;
		Coordinate<double> normalizationFactor;

		//Gets the cutoff index for a given confidence level
		int getCutoff(double confidenceLevel) const;

		//Build the normalization factor by finding the highest likelihood in each channel.
		void buildNormalizationFactor();

		//Normalize and sort the bins.
		void normalizeBins();

		//Filter out normalized bins that are negligible (see Bin.h for details about negligibility)
		void filterNormalizedBins();
		
		//Builds the total normalized likelihood from filtered normalized bins.
		void buildTotalNormalizedLikelihood();

		//Gets the boundary points for a Bin container
		std::vector<Interval<int>> getConfidenceBands(int cutoffIndex) const;
		//Sees if the bin passed in has a negligible total likelihood (meaning that double cannot 
		//store the exponentiated total likelihood)
		static bool isNegligible(const Bin& bin);
};

template<typename ChannelReader>
FirstAnalyticParticleGraph::FirstAnalyticParticleGraph(ChannelReader& channelReader, double iBeta) :
	beta(iBeta),
	totalNormalizedLikelihood(0),
	dimensions(channelReader.getChannelsRead()),
	observed(channelReader.getChannelsRead()),
	normalizationFactor(channelReader.getChannelsRead())
{
	for (int i = 0; i < dimensions; ++i)
		observed[i] = channelReader[i].getObserved();
	PhaseIterator::makeBins(bins, channelReader, dimensions, beta);
	buildNormalizationFactor();
	normalizeBins();
	filterNormalizedBins();
	buildTotalNormalizedLikelihood();
	std::sort(std::execution::par_unseq, bins.begin(), bins.end());
}

template<typename ChannelReader>
void FirstAnalyticParticleGraph::PhaseIterator::makeBins(std::vector<Bin>& bins, ChannelReader& channelReader, int dimensions, double beta)
{
	bins.reserve(powerInt(nUpper, dimensions));
	for (PhaseIterator iterator(dimensions); iterator.valid(); iterator.next())
	{
		auto current = iterator.getCurrent();
		std::vector<Bin::NMuPair> params;
		for (int i = 0; i < dimensions; ++i)
			params.emplace_back(current[i], channelReader[i].getMu(beta));
		bins.emplace_back(params);
	}
}