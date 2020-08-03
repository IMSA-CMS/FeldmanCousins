#pragma once
#include "Interval.h"
#include "BasicChannelReader.h"
#include <functional>
#include <cmath>
#include <set>
// FourthAnalyticParticleGraph.h: Creates a particle graph that builds its bins 
// outward, and builds only the bins that it has to build. Similar
// to ThirdAnalyticParticleGraph.h, but uses Sets and does not attempt
// to delete unneeded bins. This will hopefully (not just hopefully,
// quite a bit as it turns out) improve performance
// and scalability by reducing reallocations and improving on 
// repeated insertion without duplicates.
// This is the fourth, multi-channel version of SingleChannelPoissonDistribution.h.
class FourthAnalyticParticleGraph
{
	public:
		// Loads a ChannelReader into a FourthAnalyticParticleGraph class.
		// ChannelReader must have the subscript operator overloaded that returns a channel
		// Which has methods getMu and getObserved
		// As well as a method that gets the number of channels read called getChannelsRead().
		// For an implementation of this template, please see BasicChannelReader.h and
		// BasicChannel.h.
		template<typename ChannelReader>
		FourthAnalyticParticleGraph(ChannelReader& iChannelReader, double beta);

		// Check if the observed values are inside the confidence region created for a given
		// confidence level.
		bool checkConfidenceRegion(double confidenceLevel);

		// Gets the intervals of n found in the borders set when built up to the confidence level.
		std::vector<Interval<int>> getBorderIntervals(double confidenceLevel);

		// Comparison function for Coordinate<int>
		static bool lessThanCmpCoordinate(const Coordinate<int>& one, const Coordinate<int>& two);

		// Stable comparison function for Bin
		static bool lessThanCmpBin(const Bin& one, const Bin& two);
		
	private:
		int numberOfChannels;
		Coordinate<double> mu;
		Coordinate<int> observed;

		using BinSet = std::set<Bin, std::function<bool(const Bin&, const Bin&)>>;
		using CoordinateSet = std::set<Coordinate<int>, 
			std::function<bool(const Coordinate<int>&, const Coordinate<int>&)>>;

		// Stores initial bin and initial borders
		std::unique_ptr<Bin> initialBin;
		BinSet initialBorders{ lessThanCmpBin };
		
		CoordinateSet bins{ lessThanCmpCoordinate };
		BinSet borders{ lessThanCmpBin };

		// Builds the bins to the confidence level given
		void buildBins(double confidenceLevel);

		// Promotes a bin from the borders vector
		void promoteBin(double& currentConfidenceLevel);

		//Adds neighbors to the borders vector
		void addNeighbors(const Coordinate<int>& center);

		//Builds the initial borders
		void buildInitialBorders();
};

template<typename ChannelReader>
inline FourthAnalyticParticleGraph::FourthAnalyticParticleGraph(ChannelReader& iChannelReader, double beta) :
	numberOfChannels(iChannelReader.getChannelsRead()),
	mu(numberOfChannels),
	observed(numberOfChannels)
{
	std::vector<Bin::NMuPair> nMuPairs;
	nMuPairs.reserve(numberOfChannels);
	for (int i = 0; i < numberOfChannels; ++i)
	{
		const double currentMu = iChannelReader[i].getMu(beta);
		mu[i] = currentMu;
		nMuPairs.emplace_back(std::round(currentMu), currentMu);
		observed[i] = iChannelReader[i].getObserved();
	}
	initialBin = std::make_unique<Bin>(nMuPairs);
	buildInitialBorders();
}


//Necessary due to weird Visual Studio quirks
namespace dummy {}