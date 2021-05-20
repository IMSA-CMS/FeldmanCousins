#ifndef CMS_FREQUENTIST_MONTECARLO_ORTHOGONAL_PLANE_H
#define CMS_FREQUENTIST_MONTECARLO_ORTHOGONAL_PLANE_H
#include <vector>
#include "Point.h"
#include "CMS-Frequentest-MonteCarlo.h"
class Orthogonal_Plane
{
public:
	Orthogonal_Plane(std::vector<std::vector<double>> points);
	double Loss(Point testPoint);
	bool Check_Plane();
private:
	Point orthogonalPoint;
	std::vector<std::vector<double>> pointSet;
	int nDims;
	double currentLoss;
	std::vector<double> grad();
	void findStartPoint();
};
#endif