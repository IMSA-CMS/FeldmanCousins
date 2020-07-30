#pragma once
#include "Bin.h"
#include "BasicChannelReader.h"
#include "Interval.h"
#include <vector>
#include <cmath>
// SecondAnalyticParticleGraph.h: Creates a particle graph that builds all of
// its non-negligible bins outward at once with a given beta. 
// This class is the second 
// SingleChannelPoissonDistribution class generalized to N dimensions/channels,
// and builds the intervals (and hence the graph as a whole) outward
// instead of building everything, resulting in less time building.
// Sadly, the bins cannot be built all at once due to the need to normalize
// (this is before a bug in the code that computes the log-likelihood was discovered)
class SecondAnalyticParticleGraph
{
	public:
		//Loads a ChannelReader into a SecondAnalyticParticleGraph class.
		//ChannelReader must have the subscript operator overloaded that returns a channel
		//Which has methods getMu and getObserved
		//As well as a method that gets the number of channels read called getChannelsRead().
		//For an implementation of this template, please see BasicChannelReader.h and
		//BasicChannel.h.
		template<typename ChannelReader>
		SecondAnalyticParticleGraph(ChannelReader& iChannelReader, double beta);

		//Check if the observed values are inside the confidence region 
		bool checkConfidenceRegion(double confidenceLevel);
	private:
		//Stores the N's of the Bins that aren't negligible
		std::vector<Bin> bins;
		
		//Stores the current borders
		//Stores the normalization constant used to normalize probabilities
		//(sum of non-negligible bin probabilities)
		double normalizationConstant;

		Coordinate<double> mu;
		Coordinate<int> observed;
		int numberOfChannels;
		
		//Builds the initial borders
		std::vector<Bin> buildInitialBorders();

		//Builds the non-negligible bins and sets the normalization constant
		void buildBins();

		//Adds all the neighboring points to the borders set
		void addNeighbors(Coordinate<int> center, std::vector<Bin>& borders);

		//Makes intervals from the bins member with the given confidence level
		std::vector<Interval<int>> createConfidenceBandsWithConfidenceLevel(double confidenceLevel);

		//Makes intervals from the bins member until the given index
		std::vector<Interval<int>> createConfidenceBandsFromBins(int stopIndex);

		//Sets the boundary between negligible and non-negligible bins
		static constexpr int lnNegligible = -13;
};

template<typename ChannelReader>
SecondAnalyticParticleGraph::SecondAnalyticParticleGraph(ChannelReader& iChannelReader, double beta) :
	mu(iChannelReader.getChannelsRead()),
	numberOfChannels(iChannelReader.getChannelsRead()),
	observed(iChannelReader.getChannelsRead())
{
	std::vector<Bin::NMuPair> nMuPairs;
	for (int i = 0; i < numberOfChannels; ++i)
	{
		observed[i] = iChannelReader[i].getObserved();
		const double currentMu = iChannelReader[i].getMu(beta);
		mu[i] = currentMu;
		nMuPairs.emplace_back(std::round(currentMu), currentMu);
	}
	bins.emplace_back(nMuPairs);
	buildBins();
}
 
//Necessary due to weird Visual Studio quirks
namespace dummy {}