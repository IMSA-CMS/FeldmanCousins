#include "stdafx.h"
#include "Rotating_Plane.h"
#include "Constants.h"
#include <iostream>

Rotating_Plane::Rotating_Plane(std::vector<Point> points)
{
	pointSet = points;
}

int Rotating_Plane::findLargestIndex(int index)
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

int Rotating_Plane::findSmallestIndex(int index)
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

void Rotating_Plane::phiRotation(int index,char oriantation)
{
	int rotationInd;
	if (oriantation)
	{
		rotationInd = findLargestIndex(index);
	}
	else
	{
		rotationInd = findSmallestIndex(index);
	}
	for (int i = 0; i < pointSet.size(); ++i)
	{
		if (i != rotationInd)
		{
			pointSet[i].rotate_theta(pointSet[rotationInd]);
			pointSet[i].rotate_phi(pointSet[rotationInd].get_sphere(index),index);
		}
	}
	pointSet.erase(pointSet.begin() + rotationInd);
}

void Rotating_Plane::thetaRotation(int index)
{
	for (int i = 0; i < pointSet.size(); ++i)
	{
		if (i != index)
			pointSet[i].rotate_theta(pointSet[index]);
	}
	pointSet[index].rotate_theta(pointSet[index]);
}

void Rotating_Plane::removeDim(int index)
{
	for (int i = 0; i < pointSet.size(); ++i)
	{
		pointSet[i].popDim(index);
	}
}

bool Rotating_Plane::check2D()
{
	if (pointSet[0].get_dim() != 2)
	{
		try
		{
			throw;
		}
		catch (...)
		{
			std::cout << "Not two dimensions" << std::endl;
		}
	}
	//calculate the largest point with 0 < theta < pi and check if the line r=theta works
	//if not, find the smallest in the interval and do the same 
	double maxTopTheta = 0;
	double minTopTheta = Constants::pi;
	double maxBottomTheta = 0;
	double minBottomTheta = Constants::pi;

	bool inTop = false;
	bool inBottom = false;
	for (int i = 0; i < pointSet.size(); ++i)
	{
		if(pointSet[i].get_theta()>=0 && pointSet[i].get_theta() <= Constants::pi && pointSet[i].get_theta() > maxTopTheta)
		{
			maxTopTheta = pointSet[i].get_theta();
			inTop = true;
		}
		if (pointSet[i].get_theta() >= 0 && pointSet[i].get_theta() <= Constants::pi && pointSet[i].get_theta() < minTopTheta)
		{
			minTopTheta = pointSet[i].get_theta();
			inTop = true;
		}
		if (pointSet[i].get_theta() >= Constants::pi && pointSet[i].get_theta() <= 2*Constants::pi && pointSet[i].get_theta() > maxBottomTheta)
		{
			maxBottomTheta = pointSet[i].get_theta();
			inBottom = true;
		}
		if (pointSet[i].get_theta() >= Constants::pi && pointSet[i].get_theta() <= 2*Constants::pi && pointSet[i].get_theta() < minBottomTheta)
		{
			minBottomTheta = pointSet[i].get_theta();
			inBottom = true;
		}
	}
	bool inHalfPlane = false;
	if (inTop && inBottom)
	{
		if (minBottomTheta - minTopTheta <= Constants::pi || maxBottomTheta - maxTopTheta <= Constants::pi)
			inHalfPlane = true;
	}
	else if (inTop && !inBottom)
		inHalfPlane = true;
	else if (!inTop && inBottom)
		inHalfPlane = true;
	return inHalfPlane;
}
//The current shiftString solution goes through all 2^n possibilities
//A potentially better solution may be to check if one of the half-spaces defined by the plane orthogonal to the highest dimenstions fits in an N-1 dimensional half-space
bool Rotating_Plane::checkingPoints(unsigned int shiftString) 
{
	auto tempSet = Rotating_Plane(pointSet);
	const int originalDim = pointSet[0].get_dim();
	int dimNum = pointSet[0].get_dim();

	for (int i = 0; i < dimNum; ++i)
	{
		unsigned int currentDim = pow(2, i);
		tempSet.thetaRotation(findLargestIndex(i));
		if (shiftString & currentDim)
			tempSet.phiRotation(0, 1);
		else
			tempSet.phiRotation(0, 0);
		tempSet.removeDim(0);
	}
	if (tempSet.check2D())
		return true;
	else if (shiftString >= pow(2, originalDim) - 1)
		return false;
	else
		return checkingPoints(shiftString + 1);
}