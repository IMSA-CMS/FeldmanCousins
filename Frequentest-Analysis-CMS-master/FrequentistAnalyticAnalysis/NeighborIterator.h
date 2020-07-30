#pragma once
#include "Coordinate.h"
//This class helps iterate over the neighboring coordinates around a central coordinate
class NeighborIterator
{
	public:
		//Builds the NeighborIterator with the central coordinate
		NeighborIterator(const Coordinate<int>& iCenter);

		//Goes to the next iteration.
		void next();

		//Tests if the iterator is exhausted.
		bool isExhausted() const;

		//Gets the current iteration
		const Coordinate<int>& getCurrent();
	private:
		//Central point with one subtracted from each dimension
		Coordinate<int> currentIteration;

		//Stores the position of the current iteration
		std::valarray<int> position;

		//As the iteration starts at the smallest neighbor, the dimension
		//Gets to be its largest when it is added to twice.
		static constexpr int maxPosition = 2;

		const Coordinate<int>& center;
};

