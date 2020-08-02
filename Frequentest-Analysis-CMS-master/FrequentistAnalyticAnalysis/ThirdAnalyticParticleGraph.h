#pragma once
#include "Bin.h"
#include "Interval.h"
#include "BasicChannelReader.h"
#include <memory>
// ThirdAnalyticParticleGraph.h: Creates a particle graph that builds its bins 
// outward, and builds only the bins that it has to build. This is possible
// due to the fixed poisson calculation code. Only stores the border region
// as well as the parents to all of the bins in the border region. All of these
// optimizations result in faster compute times (due to find operations on less items) and 
// lower memory utilization than the SecondAnalyticParticleGraph.h. This is the third,
// multi-channel version of SingleChannelPoissonDistribution.h. This method scales 
// very poorly in N, and is rewritten in the FourthAnalyticParticleGraph.h.
class ThirdAnalyticParticleGraph
{
	public:
		//Loads a ChannelReader into a ThirdAnalyticParticleGraph class.
		//ChannelReader must have the subscript operator overloaded that returns a channel
		//Which has methods getMu and getObserved
		//As well as a method that gets the number of channels read called getChannelsRead().
		//For an implementation of this template, please see BasicChannelReader.h and
		//BasicChannel.h.
		template<typename ChannelReader>
		ThirdAnalyticParticleGraph(ChannelReader& iChannelReader, double beta);

		//Check if the observed values are inside the confidence region created for a given
		//confidence level.
		bool checkConfidenceRegion(double confidenceLevel);

		//Gets the intervals of n found in the borders vector when built up to the confidence level.
		std::vector<Interval<int>> getBorderIntervals(double confidenceLevel);

		//Ensures that the border vectors get cleaned up before the parent store
		//(otherwise, the Parent class will not be able to destroy itself properly)
		virtual ~ThirdAnalyticParticleGraph();
	private:
		int numberOfChannels;
		Coordinate<double> mu;
		Coordinate<int> observed;

		//Stores a parent of a border inside the border vector
		//Manages the parentStore provided to ensure no dangling pointers
		class Parent
		{
			public:
				//Gets the parent bin stored inside Parent
				Coordinate<int>& getBin() { return *parent; }
				const Coordinate<int>& getBin() const { return *parent; }

				//Builds the parent using a coordinate and the store of all of the parents
				Parent(const Coordinate<int>& iParent, std::vector<Coordinate<int>*>* parentStore);
			private:
				std::shared_ptr<Coordinate<int>> parent;
		};

		//Border struct built for easy access to border elements
		struct Border
		{
			Border(const Bin& iBorderBin, const Parent& iParent) : borderBin(iBorderBin), parent(iParent) {}
			Bin borderBin;
			bool operator< (Border& other) { return borderBin < other.borderBin; }
			private:
				Parent parent;
		};

		std::vector<Border> borders;
		//Stores the initial border
		std::vector<Border> initialBorders;
		//Stores pointers to all of the parents
		std::vector<Coordinate<int>*> parentStore;

		//Builds the initial borders
		void buildInitialBorders(const Parent& initial);

		//Adds neighbors of the parent to the borders vector.
		void addNeighbors(const Parent& parent);

		//Checks if the bin is present in borders and parentStore
		bool isPresent(const Coordinate<int>& bin);
		
		//Builds the bins up to the confidence level.
		void buildBins(double confidenceLevel);
};

template<typename ChannelReader>
ThirdAnalyticParticleGraph::ThirdAnalyticParticleGraph(ChannelReader& iChannelReader, double beta) :
	numberOfChannels(iChannelReader.getChannelsRead()),
	mu(numberOfChannels),
	observed(numberOfChannels)
{
	Coordinate<int> initialBin(numberOfChannels);
	for (int i = 0; i < numberOfChannels; ++i)
	{
		const double currentMu = iChannelReader[i].getMu(beta);
		mu[i] = currentMu;
		observed[i] = iChannelReader[i].getObserved();
		initialBin[i] = std::round(currentMu);
	}
	buildInitialBorders(Parent(initialBin, &parentStore));
}

//Necessary due to weird Visual Studio quirks
namespace dummy {}