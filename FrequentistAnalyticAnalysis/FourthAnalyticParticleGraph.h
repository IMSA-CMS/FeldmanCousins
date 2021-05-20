#ifndef FREQUENTIST_ANALYTIC_ANALYSIS_FOURTHANALYTICPARTICLEGRAPH_H
#define FREQUENTIST_ANALYTIC_ANALYSIS_FOURTHANALYTICPARTICLEGRAPH_H
#include "Interval.h"
#include "BasicChannelReader.h"
#include <functional>
#include <cmath>
#include "FourthBorderGraph.h"
#include "FourthBinGraph.h"
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
		
	private:
		int numberOfChannels;
		Coordinate<double> mu;
		Coordinate<int> observed;
		
		FourthBinGraph binGraph;
		FourthBorderGraph borderGraph;

		// Builds the bins to the confidence level given
		void buildBins(double confidenceLevel);

		// Promotes a bin from the border graph
		void promoteBin(double& currentConfidenceLevel);

		//Adds neighbors to the border graph
		void addNeighbors(Coordinate<int> center);
};

template<typename ChannelReader>
inline FourthAnalyticParticleGraph::FourthAnalyticParticleGraph(ChannelReader& iChannelReader, double beta) :
	numberOfChannels(iChannelReader.getChannelsRead()),
	mu(numberOfChannels),
	observed(numberOfChannels),
	binGraph(numberOfChannels)
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
	Bin initialBin{ nMuPairs };
	binGraph.setInitialBin(initialBin);
	borderGraph.buildInitialBorders(initialBin.getAllN(), mu);
}


//Necessary due to weird Visual Studio quirks
namespace dummy {}
#endif