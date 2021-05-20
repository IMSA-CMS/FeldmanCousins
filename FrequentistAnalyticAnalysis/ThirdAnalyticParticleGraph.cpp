#include "ThirdAnalyticParticleGraph.h"
#include "NeighborIterator.h"
#include <algorithm>
#include <execution>
#include <iostream>
ThirdAnalyticParticleGraph::Parent::Parent(const Coordinate<int>& iParent, std::vector<Coordinate<int>*>* parentStore)
{
	auto parentDeleter = [parentStore](Coordinate<int>* parent)
	{
		auto parentLocation = std::find(std::execution::par_unseq, parentStore->begin(), parentStore->end(), parent);
		parentStore->erase(parentLocation);
		delete parent;
	};
	parent = std::shared_ptr<Coordinate<int>>(new Coordinate<int>(iParent), parentDeleter);
	parentStore->push_back(parent.get());
}

bool ThirdAnalyticParticleGraph::checkConfidenceRegion(double confidenceLevel)
{
	auto intervals = getBorderIntervals(confidenceLevel);
	for (int i = 0; i < numberOfChannels; ++i)
		if (!intervals[i].insideInterval(observed[i])) return false;
	return true;
}

std::vector<Interval<int>> ThirdAnalyticParticleGraph::getBorderIntervals(double confidenceLevel)
{
	buildBins(confidenceLevel);
	std::vector<Interval<int>> intervals;
	for (int i = 0; i < numberOfChannels; ++i)
		intervals.emplace_back(borders.front().borderBin.getN(i), borders.front().borderBin.getN(i));
	for (auto border = borders.begin() + 1; border < borders.end(); ++border)
	{
		for (int i = 0; i < numberOfChannels; ++i)
			intervals[i].widen(Interval<int>(border->borderBin.getN(i), border->borderBin.getN(i)));
	}
	return intervals;
}

ThirdAnalyticParticleGraph::~ThirdAnalyticParticleGraph()
{
	borders.clear();
	initialBorders.clear();
}

void ThirdAnalyticParticleGraph::buildInitialBorders(const ThirdAnalyticParticleGraph::Parent& initial)
{
	std::vector<Bin::NMuPair> nMuPairs;
	for (NeighborIterator iterator(initial.getBin()); !iterator.isExhausted(); iterator.next())
	{
		auto current = iterator.getCurrent();
		for (int i = 0; i < numberOfChannels; ++i)
			nMuPairs.emplace_back(current[i], mu[i]);
		initialBorders.emplace_back(Bin(nMuPairs), initial);
		nMuPairs.clear();
	}
}

void ThirdAnalyticParticleGraph::addNeighbors(const ThirdAnalyticParticleGraph::Parent& parent)
{
	std::vector<Bin::NMuPair> nMuPairs;
	for (NeighborIterator iterator(parent.getBin()); !iterator.isExhausted(); iterator.next())
	{
		auto current = iterator.getCurrent();
		if (isPresent(current)) continue;
		for (int i = 0; i < numberOfChannels; ++i)
			nMuPairs.emplace_back(current[i], mu[i]);
		borders.emplace_back(Bin(nMuPairs), parent);
		nMuPairs.clear();
	}
}

bool ThirdAnalyticParticleGraph::isPresent(const Coordinate<int>& bin)
{
	auto borderEquals = [&bin](const Border& other) {return bin == other.borderBin.getAllN();};
	if (std::find_if(std::execution::par_unseq, borders.begin(), borders.end(), borderEquals) != borders.end())
		return true;
	auto parentEquals = [&bin](const Coordinate<int>* other) {return bin == *other;};
	if (std::find_if(std::execution::par_unseq, parentStore.begin(), parentStore.end(), parentEquals)
		!= parentStore.end())
		return true;
	return false;
}

void ThirdAnalyticParticleGraph::buildBins(double confidenceLevel)
{
	borders = initialBorders;
	volatile double totalLikelihood = 0;
	while (totalLikelihood <= confidenceLevel)
	{
		int promotedIndex = std::max_element(borders.begin(), borders.end()) - borders.begin();
		totalLikelihood += std::exp(borders[promotedIndex].borderBin.getTotalLnLikelihood());
		Parent promoted(borders[promotedIndex].borderBin.getAllN(), &parentStore);
		addNeighbors(promoted);
		borders.erase(borders.begin() + promotedIndex);
	}
}

