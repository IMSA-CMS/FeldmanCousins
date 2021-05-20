#ifndef CMS_FREQUENTIST_MONTECARLO_PLANECHECKER_H
#define CMS_FREQUENTIST_MONTECARLO_PLANECHECKER_H
#include <vector>
#include "Point.h"
#include "CMS-Frequentest-MonteCarlo.h"
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
#endif
