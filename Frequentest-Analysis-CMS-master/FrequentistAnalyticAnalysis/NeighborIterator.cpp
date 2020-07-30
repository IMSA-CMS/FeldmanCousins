#include "NeighborIterator.h"

NeighborIterator::NeighborIterator(const Coordinate<int>& iCenter) :
	currentIteration(iCenter.getDimensions()),
	position(0,iCenter.getDimensions()),
	center(iCenter)
{
	for (int i = 0; i < currentIteration.getDimensions(); ++i)
		currentIteration[i] = center[i] - 1;
}

void NeighborIterator::next()
{
	int currentPosition = position.size() - 1;
	++(currentIteration[currentPosition]);
	++(position[currentPosition]);
	while (position[currentPosition] > 2 && currentPosition > 0)
	{
		++(currentIteration[currentPosition - 1]);
		++(position[currentPosition - 1]);
		currentIteration[currentPosition] -= (maxPosition + 1);
		position[currentPosition] = 0;
		--currentPosition;
	}
}

bool NeighborIterator::isExhausted() const
{
	return position[0] > maxPosition;
}

const Coordinate<int>& NeighborIterator::getCurrent()
{
	if (currentIteration == center) next();
	return currentIteration;
}
