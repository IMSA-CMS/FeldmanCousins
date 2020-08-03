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
	std::vector<Interval<int>> intervals;
	for (int i = 0; i < numberOfChannels; ++i)
		intervals.emplace_back(borders.begin()->getN(i), borders.begin()->getN(i));
	for (const Bin& border : borders)
	{
		for (int i = 0; i < numberOfChannels; ++i)
			intervals[i].widen(Interval(border.getN(i), border.getN(i)));
	}
	return intervals;
}
bool FourthAnalyticParticleGraph::lessThanCmpCoordinate(const Coordinate<int>& one, const Coordinate<int>& two)
{
	if (one.getDimensions() != two.getDimensions())
		throw std::runtime_error("Bins are not comparable as they do not have the same number of dimensions.");
	for (int i = 0; i < one.getDimensions(); ++i)
	{
		if (one[i] == two[i]) continue;
		return one[i] < two[i];
	}
	return false;
}
bool FourthAnalyticParticleGraph::lessThanCmpBin(const Bin& one, const Bin& two)
{
	if (one.getAllN() == two.getAllN())
		return false;
	return one < two;
}
void FourthAnalyticParticleGraph::buildBins(double confidenceLevel)
{
	bins.clear();
	bins.insert(initialBin->getAllN());
	borders = initialBorders;
	double currentConfidenceLevel = std::exp(initialBin->getTotalLnLikelihood());
	while (currentConfidenceLevel <= confidenceLevel)
		promoteBin(currentConfidenceLevel);
}

void FourthAnalyticParticleGraph::promoteBin(double& currentConfidenceLevel)
{
	auto promoted = std::prev(borders.end());
	addNeighbors(promoted->getAllN());
	currentConfidenceLevel += std::exp(promoted->getTotalLnLikelihood());
	bins.insert(promoted->getAllN());
	borders.erase(promoted);
}

void FourthAnalyticParticleGraph::addNeighbors(const Coordinate<int>& center)
{
	std::vector<Bin::NMuPair> nMuPairs;
	nMuPairs.reserve(numberOfChannels);
	for (NeighborIterator iterator(center); !iterator.isExhausted(); iterator.next())
	{
		auto current = iterator.getCurrent();
		if (bins.find(current) != bins.end()) continue;
		for (int i = 0; i < numberOfChannels; ++i)
			nMuPairs.emplace_back(current[i], mu[i]);
		borders.emplace(nMuPairs);
		nMuPairs.clear();
	}
}

void FourthAnalyticParticleGraph::buildInitialBorders()
{
	std::vector<Bin::NMuPair> nMuPairs;
	nMuPairs.reserve(numberOfChannels);
	for (NeighborIterator iterator(initialBin->getAllN()); !iterator.isExhausted(); iterator.next())
	{
		auto current = iterator.getCurrent();
		for (int i = 0; i < numberOfChannels; ++i)
		{
			nMuPairs.emplace_back(current[i], mu[i]);
		}
		initialBorders.emplace(nMuPairs);
		nMuPairs.clear();
	}
}
