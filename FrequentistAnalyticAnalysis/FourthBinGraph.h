#ifndef FREQUENTIST_ANALYTIC_ANALYSIS_FOURTHBINGRAPH_H
#define FREQUENTIST_ANALYTIC_ANALYSIS_FOURTHBINGRAPH_H
#include "Bin.h"
#include <vector>
#include <optional>
#include <set>
#include "Coordinate.h"
#include <functional>
// FourthBinGraph.h: Although this isn't the fourth graph set per se, it was developed "in conjunction"
// with FourthAnalyticParticleGraph.h. This happened after Dr. Peter Dong asked me to abstract away
// all implementations of containers, which is advice you should follow, in my opinion. Contains
// a bunch of abstracted-away container calls useful to FourthAnalyticParticleGraph.h (see FourthAnalytic
// ParticleGraph.h for more details) that deal with the bin graph.
class FourthBinGraph
{
	public:
		// Builds an empty FourthBinGraph, without an initial bin
		FourthBinGraph(int iDimensions) : dimensions(iDimensions) { }

		// Sets the initial bin from nMuPairs
		void setInitialBin(const Bin& iInitialBin);

		// Gets the start likelihood to begin building the rest of the bins
		double getStartLnLikelihood() { return startLnLikelihood.value(); }

		// Gets the initial bin
		const Coordinate<int>& getInitialBin() { return initialBin.value(); }

		// Adds a new bin to the binSet.
		void add(const Coordinate<int>& bin);

		// Sees if a bin is in the bin set.
		bool contains(const Coordinate<int>& coordinate);

		// Resets the binSet to only contain the initial bin.
		void reset();

		//Prints all of the values in binSet for debugging
		void printValuesDebug();

		// Gets the dimensions this class was created with.
		int getDimensions();
	private:
		std::optional<Coordinate<int>> initialBin;
		std::set<Coordinate<int>, std::function<bool(const Coordinate<int>&, const Coordinate<int>&)>>
			binSet { &Coordinate<int>::lessThanRowMajor };
		int dimensions;
		std::optional<double> startLnLikelihood;
};
#endif
