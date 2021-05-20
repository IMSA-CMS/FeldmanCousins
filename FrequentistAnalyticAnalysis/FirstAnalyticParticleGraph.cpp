#include "FirstAnalyticParticleGraph.h"
#include "Coordinate.h"
#include <fstream>
#include <cmath>
#include <utility>
#include <algorithm>
#include <execution>
#include <atomic>
//Begin PhaseIterator code
FirstAnalyticParticleGraph::PhaseIterator::PhaseIterator(int dims) :
	currArray(dims),
	endArray(dims)
{
	for (int i = 0; i < dims; ++i)
	{
		currArray[i] = 0;
		endArray[i] = nUpper;
	}
}

const Coordinate<int>& FirstAnalyticParticleGraph::PhaseIterator::getCurrent()
{
	return currArray;
}

void FirstAnalyticParticleGraph::PhaseIterator::next()
{
	++currArray[currArray.getDimensions() - 1];
	for (int i = currArray.getDimensions() - 1; i > 0; --i)
	{
		if (currArray[i] == endArray[i])
		{
			currArray[i] = 0;
			++currArray[i - 1];
		}
	}
}

bool FirstAnalyticParticleGraph::PhaseIterator::valid()
{
	return currArray[0] < endArray[0];
}


int FirstAnalyticParticleGraph::PhaseIterator::powerInt(int base, int exponent)
{
	int result = 1;
	while (exponent)
	{
		if (exponent & 1)
			result *= base;
		exponent >>= 1;
		base *= base;
	}
	return result;
}

//End PhaseIterator Code
bool FirstAnalyticParticleGraph::checkConfidenceRegion(double confidenceLevel) const
{
	int cutoff = getCutoff(confidenceLevel);
	auto confidenceBands = getConfidenceBands(cutoff);
	for (int i = 0; i < confidenceBands.size(); ++i)
		if (!(confidenceBands[i].insideInterval(observed[i]))) return false;
	return true;
}

int FirstAnalyticParticleGraph::getCutoff(double confidenceLevel) const
{
	double integratedProb = 0;
	for (int i = bins.size() - 1; i >= 0; --i)
	{
		if (integratedProb > (confidenceLevel*totalNormalizedLikelihood)) 
			return i;
		integratedProb += std::exp(bins[i].getTotalLnLikelihood());
	}
	return 0;
}

void FirstAnalyticParticleGraph::filterNormalizedBins()
{
	auto removeIterator = std::remove_if(std::execution::par_unseq, bins.begin(), bins.end(), 
		FirstAnalyticParticleGraph::isNegligible);
	const int elementsRemoved = std::distance(removeIterator, bins.end());
	for (int i = 0; i < elementsRemoved; ++i)
		bins.pop_back();
}

void FirstAnalyticParticleGraph::buildTotalNormalizedLikelihood()
{
	for (const auto& bin : bins)
		totalNormalizedLikelihood += std::exp(bin.getTotalLnLikelihood());
}

/*
void FirstAnalyticParticleGraph::normalizeBins()
{
	std::atomic<const Coordinate<double>*> factor(&normalizationFactor);
	std::for_each(std::execution::par_unseq, bins.begin(), bins.end(), [&factor](Bin& b) {b.normalize(*factor);});
}

void FirstAnalyticParticleGraph::buildNormalizationFactor()
{
	for (int i = 0; i < dimensions; ++i)
	{
		auto comparisonFunction = 
		[i](const Bin& one, const Bin& two) {return one.getLnLikelihood(i) < two.getLnLikelihood(i);};
		auto max = std::max_element(std::execution::par_unseq, bins.begin(), bins.end(), comparisonFunction);
		normalizationFactor[i] = max->getLnLikelihood(i);
	}
}
*/
std::vector<Interval<int>> FirstAnalyticParticleGraph::getConfidenceBands(int cutoffIndex) const
{
	std::vector<Interval<int>> confidenceBands;
	for (int i = 0; i < dimensions; ++i)
		confidenceBands.emplace_back(bins[cutoffIndex].getN(i), bins[cutoffIndex].getN(i));
	for (int i = cutoffIndex; i < bins.size(); ++i)
	{
		for (int j = 0; j < confidenceBands.size(); ++j)
		{
			Interval point(bins[i].getN(j), bins[i].getN(j));
			confidenceBands[j].widen(point);
		}
	}
	return confidenceBands;
}


bool FirstAnalyticParticleGraph::isNegligible(const Bin& bin)
{
	constexpr int lowerLogBound = -700;  // Close to the lowest representable double value
	return bin.getTotalLnLikelihood() < lowerLogBound;
}
