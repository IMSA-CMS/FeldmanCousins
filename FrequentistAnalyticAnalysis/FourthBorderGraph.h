#ifndef FREQUENTIST_ANALYTIC_ANALYSIS_FOURTHBORDERGRAPH_H
#define FREQUENTIST_ANALYTIC_ANALYSIS_FOURTHBORDERGRAPH_H
#include "Bin.h"
#include <set>
#include "Interval.h"
// FourthBorderGraph.h: Although this isn't the fourth graph per se, it was developed "in conjunction"
// with FourthAnalyticParticleGraph.h. This happened after Dr. Peter Dong asked me to abstract away
// all implementations of containers, which is advice you should follow, in my opinion. Contains
// a bunch of abstracted-away container calls useful to FourthAnalyticParticleGraph.h (see FourthAnalytic
// ParticleGraph.h for more details) that deal with the border graph.
class FourthBorderGraph
{
	public:
		// Builds empty FourthBorderGraph, without initial borders.
		FourthBorderGraph() {}

		// Builds the initial Borders from the initial Bin.
		void buildInitialBorders(const Coordinate<int>& iInitialBin, const Coordinate<double> mu);

		// Pops the bin with the highest likelihood.
		Bin pop();

		// Adds a new Bin to the border graph.
		void add(const Bin& bin);

		// Resets the FourthBorderGraph so that it only has the initial borders.
		void reset();

		//Gets the intervals of the borders
		std::vector<Interval<int>> getBorderIntervals();

		//Prints the values to be used for debugging
		void printValuesDebug();
	private:
		struct BinComparison
		{
			bool operator()(const Bin& one, const Bin& two) const;
		};
		std::set<Bin, BinComparison> borders, initialBorders;
};
#endif
