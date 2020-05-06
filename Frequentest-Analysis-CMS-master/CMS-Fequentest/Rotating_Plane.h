#pragma once
#include <vector>
#include "point.h"
#include "CMS-Frequentest-Analysis.h"
class Rotating_Plane
{
public:
	Rotating_Plane(std::vector<Point> points);
	//Returns the index of the point with the largest spherical[index]
	int findLargestIndex(int index);
	//Returns the index of the point with the smallest spherical[index]
	int findSmallestIndex(int index);
	//Rotates all points in the plane in such a way that the highest point is on the index axis
	//if oriantation = 1 and lowest point if oriantation = 0
	void phiRotation(int index,char oriantation);
	//Rotates all points in the plane in the theta direction in such a way that the indexed value
	//has theta value 0
	void thetaRotation(int index);
	//Removes a dimension from all points in points list
	void removeDim(int index);
	//Checks if a 2D system is on a half-plane
	bool check2D();
	//Repeatedly rotates the plane and checks if each rotation results in a division. If one does, returns true.
	bool checkingPoints(unsigned int shiftString = 0);

private:
	std::vector<Point> pointSet;

};

