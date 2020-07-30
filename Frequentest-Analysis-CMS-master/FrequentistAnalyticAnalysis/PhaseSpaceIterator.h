#pragma once
#include <vector>
class PhaseSpaceIterator
{
	public:
		const std::vector<int>& getCurrent();
		void next();
		bool valid();
		PhaseSpaceIterator(int dimensions);
	private:
		const int nUpperAll = 1000;
		std::vector<int> currentPoint, endPoint;
};