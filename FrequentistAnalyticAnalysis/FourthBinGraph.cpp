#include "FourthBinGraph.h"
#include <iostream>

void FourthBinGraph::setInitialBin(const Bin& iInitialBin)
{
	initialBin = iInitialBin.getAllN();
	binSet.insert(iInitialBin.getAllN());
	startLnLikelihood = iInitialBin.getTotalLnLikelihood();
}

void FourthBinGraph::add(const Coordinate<int>& bin)
{
	binSet.insert(bin);
}

bool FourthBinGraph::contains(const Coordinate<int>& coordinate)
{
	return binSet.find(coordinate) != binSet.end();
}

void FourthBinGraph::reset()
{
	binSet.clear();
	binSet.insert(initialBin.value());
}

void FourthBinGraph::printValuesDebug()
{
	for (const Coordinate<int>& b : binSet)
	{
		std::cerr << '[';
		for (int i = 0; i < b.getDimensions(); ++i)
		{
			std::cerr << b[i] << ',';
		}
		std::cerr << ']' << ',';
	}
}

int FourthBinGraph::getDimensions()
{
	return dimensions;
}
