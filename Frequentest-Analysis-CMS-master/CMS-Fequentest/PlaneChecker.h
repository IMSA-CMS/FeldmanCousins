#pragma once
#include <vector>
#include "point.h"
#include "CMS-Frequentest-Analysis.h"
class PlaneChecker
{
public:
	PlaneChecker(std::vector<Point> points);
	int findLargestIndex(int index);
	int findSmallestIndex(int index);
	bool existsHalfSpace();
private:
	std::vector<Point> pointSet;
};

