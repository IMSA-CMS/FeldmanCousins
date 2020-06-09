#include "stdafx.h"
#include "PlaneChecker.h"
#include "Constants.h"
#include "Orthogonal_Plane.h"
#include <iostream>
PlaneChecker::PlaneChecker(std::vector<Point> points)
{
	pointSet = points;
}
int PlaneChecker::findLargestIndex(int index)
{
	int i = 0;
	int returnInd;
	double maxVal = -Constants::pi / 2;
	while (i < pointSet.size())
	{
		if (pointSet[i].get_sphere(index) > maxVal)
		{
			maxVal = pointSet[i].get_sphere(index);
			returnInd = i;
		}
		++i;
	}
	return returnInd;
}

int PlaneChecker::findSmallestIndex(int index)
{
	int i = 0;
	int returnInd;
	double minVal = Constants::pi / 2;
	while (i < pointSet.size())
	{
		if (pointSet[i].get_sphere(index) < minVal)
		{
			minVal = pointSet[i].get_sphere(index);
			returnInd = i;
		}
		++i;
	}
	return returnInd;
}

bool PlaneChecker::existsHalfSpace()
{
	for (int i = 0; i < pointSet[0].get_dim() - 1;++i)
	{
		int s = findSmallestIndex(i);
		Point orthoVect = pointSet[s];
		orthoVect.update_sphere(i, pointSet[s].get_sphere(i)+Constants::pi/2);
		Orthogonal_Plane ortho{ orthoVect.get_Cartesian() };
		if (ortho.checkPlane(pointSet))
		{
			return true;
		}
		int l = findLargestIndex(i);
		orthoVect = pointSet[l];
		orthoVect.update_sphere(i, pointSet[l].get_sphere(i) - Constants::pi / 2);
		Orthogonal_Plane ortho2{ orthoVect.get_Cartesian() };
		if (ortho2.checkPlane(pointSet))
		{
			return true;
		}

	}
	return false;
}

