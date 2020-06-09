#pragma once
#include <vector>
#include "point.h"
#include "CMS-Frequentest-Analysis.h"
class Orthogonal_Plane
{
public:
	Orthogonal_Plane(std::vector<vector<double>> points);
	double Loss(Point testPoint);
	bool Check_Plane();
private:
	Point orthogonalPoint;
	vector<vector<double>> pointSet;
	int nDims;
	double currentLoss;
	vector<double> grad();
	void findStartPoint();
};

