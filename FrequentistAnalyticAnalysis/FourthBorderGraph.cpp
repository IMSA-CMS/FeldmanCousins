#include "FourthBorderGraph.h"
#include "NeighborIterator.h"
#include <iostream>

bool FourthBorderGraph::BinComparison::operator()(const Bin& one, const Bin& two) const
{
	if (one.getN(0) == 0 || two.getN(0) == 0)
		int i = 42;
	if (one.getAllN() == two.getAllN())
		return one.getAllN().lessThanRowMajor(two.getAllN());
	return one < two;
}

void FourthBorderGraph::buildInitialBorders(const Coordinate<int>& iInitialBin, const Coordinate<double> mu)
{
	std::vector<Bin::NMuPair> nMuPairs;
	nMuPairs.reserve(iInitialBin.getDimensions());
	for (NeighborIterator iterator(iInitialBin); !iterator.isExhausted(); iterator.next())
	{
		auto current = iterator.getCurrent();
		for (int i = 0; i < iInitialBin.getDimensions(); ++i)
		{
			nMuPairs.emplace_back(current[i], mu[i]);
		}
		initialBorders.emplace(nMuPairs);
		nMuPairs.clear();
	}
	borders = initialBorders;
}

Bin FourthBorderGraph::pop()
{
	auto poppedIterator = std::prev(borders.end());
	Bin poppedBin = *poppedIterator;
	borders.erase(poppedIterator);
	return poppedBin;
}

void FourthBorderGraph::add(const Bin& bin)
{
	borders.insert(bin);
}

void FourthBorderGraph::reset()
{
	borders = initialBorders;
}

std::vector<Interval<int>> FourthBorderGraph::getBorderIntervals()
{
	int numberOfChannels = borders.begin()->getDimensions();
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

void FourthBorderGraph::printValuesDebug()
{
	std::cerr << '\n';
	for (const Bin& b : borders)
	{
		std::cerr << '[';
		for (int i = 0; i < b.getDimensions(); ++i)
		{
			std::cerr << b.getN(i) << ',';
		}
		std::cerr << ']' << ',';
	}
}