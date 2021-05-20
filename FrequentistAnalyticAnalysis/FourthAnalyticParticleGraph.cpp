#include "FourthAnalyticParticleGraph.h"
#include "NeighborIterator.h"
#include <algorithm>
#include <execution>
#include <exception>
#include <iostream>
bool FourthAnalyticParticleGraph::checkConfidenceRegion(double confidenceLevel)
{
	auto intervals = getBorderIntervals(confidenceLevel);
	for (int i = 0; i < numberOfChannels; ++i)
	{
		if (!(intervals[i].insideInterval(observed[i]))) return false;
	}
	return true;
}
std::vector<Interval<int>> FourthAnalyticParticleGraph::getBorderIntervals(double confidenceLevel)
{
	buildBins(confidenceLevel);
	return borderGraph.getBorderIntervals();
}

void FourthAnalyticParticleGraph::buildBins(double confidenceLevel)
{
	binGraph.reset();
	borderGraph.reset();
	double currentConfidenceLevel = std::exp(binGraph.getStartLnLikelihood());
	while (currentConfidenceLevel <= confidenceLevel)
		promoteBin(currentConfidenceLevel);
}

void FourthAnalyticParticleGraph::promoteBin(double& currentConfidenceLevel)
{
	Bin promoted = borderGraph.pop();
	addNeighbors(promoted.getAllN());
	currentConfidenceLevel += std::exp(promoted.getTotalLnLikelihood());
	binGraph.add(promoted.getAllN());
}

void FourthAnalyticParticleGraph::addNeighbors(Coordinate<int> center)
{
	std::vector<Bin::NMuPair> nMuPairs;
	nMuPairs.reserve(numberOfChannels);
	for (NeighborIterator iterator(center); !iterator.isExhausted(); iterator.next())
	{
		auto current = iterator.getCurrent();
		if (binGraph.contains(current)) continue;
		for (int i = 0; i < numberOfChannels; ++i)
			nMuPairs.emplace_back(current[i], mu[i]);
		borderGraph.add(Bin{ nMuPairs });
		nMuPairs.clear();
	}
}
