#include "SecondAnalyticParticleGraph.h"
#include "NeighborIterator.h"
#include <algorithm>
#include <execution>
#include <cmath>
#include <iostream>
bool SecondAnalyticParticleGraph::checkConfidenceRegion(double confidenceLevel)
{
	auto confidenceBands = createConfidenceBandsWithConfidenceLevel(confidenceLevel);
	for (int i = 0; i < numberOfChannels; ++i)
	{
		if (!confidenceBands[i].insideInterval(observed[i]))
			return false;
	}
	return true;
}

std::vector<Bin> SecondAnalyticParticleGraph::buildInitialBorders()
{
	std::vector<Bin> borders;
	for (NeighborIterator iterator(bins.front().getAllN()); !iterator.isExhausted(); iterator.next())
	{
		auto current = iterator.getCurrent();
		std::vector<Bin::NMuPair> nMuPairs;
		for (int i = 0; i < numberOfChannels; ++i)
			nMuPairs.emplace_back(current[i], mu[i]);
		borders.emplace_back(nMuPairs);
	}
	return borders;
}

void SecondAnalyticParticleGraph::buildBins()
{
	auto borders = buildInitialBorders();
	while (bins.back().getTotalLnLikelihood() > lnNegligible)
	{
		auto promoted = std::max_element(std::execution::par_unseq, borders.begin(), borders.end());
		normalizationConstant += std::exp(promoted->getTotalLnLikelihood());
		bins.push_back(std::move(*promoted));
		borders.erase(promoted);
		addNeighbors(bins.back().getAllN(), borders);
	}
}


void SecondAnalyticParticleGraph::addNeighbors(Coordinate<int> center, std::vector<Bin>& borders)
{ 
	for (NeighborIterator iterator(center); !iterator.isExhausted(); iterator.next())
	{
		auto current = iterator.getCurrent();
		auto equalsCurrent = [&current](const Bin& other) {return other.getAllN() == current;};
		if (std::find_if(std::execution::par_unseq, bins.begin(), bins.end(), equalsCurrent) != bins.end()) continue;
		std::vector<Bin::NMuPair> nMuPairs;
		for (int i = 0; i < numberOfChannels; ++i)
			nMuPairs.emplace_back(current[i], mu[i]);
		Bin bin(nMuPairs);
		auto equalsBin = [&bin](const Bin& other) {return other.getAllN() == bin.getAllN();};
		if (std::find_if(std::execution::par_unseq, borders.begin(), borders.end(), equalsBin) == borders.end()) 
			borders.push_back(std::move(bin));
	}
}

std::vector<Interval<int>> SecondAnalyticParticleGraph::createConfidenceBandsWithConfidenceLevel(double confidenceLevel)
{
	int stopIndex = 0;
	for (double integratedLikelihood = 0; stopIndex < bins.size(); ++stopIndex)
	{
		if (integratedLikelihood > confidenceLevel * normalizationConstant) break;
		integratedLikelihood += std::exp(bins[stopIndex].getTotalLnLikelihood());
	}
	return createConfidenceBandsFromBins(stopIndex);
}

std::vector<Interval<int>> SecondAnalyticParticleGraph::createConfidenceBandsFromBins(int stopIndex)
{
	std::vector<Interval<int>> confidenceBands;
	for (int i = 0; i < numberOfChannels; ++i)
		confidenceBands.emplace_back(bins.front().getN(i), bins.front().getN(i));
	for (int i = 0; i < stopIndex; ++i)
	{
		for (int j = 0; j < numberOfChannels; ++j)
			confidenceBands[j].widen(Interval(bins[i].getN(j), bins[i].getN(j)));
	}
	return confidenceBands;
}
