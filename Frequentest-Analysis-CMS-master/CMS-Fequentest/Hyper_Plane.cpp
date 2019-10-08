#include "stdafx.h"
#include <algorithm>
#include "Hyper_Plane.h"
#include "CMS-Frequentest-Analysis.h"




Hyper_Plane::Hyper_Plane(std::vector<std::vector<double>> adataSet)
{
	basePlane = true;
	int SIZE_OF_DATA_SET = adataSet.size();
	int NUMBER_OF_DIMENSIONS = adataSet[0].size();
	std::vector<std::vector<double>> dataSet = adataSet;
	std::sort(dataSet.begin(), dataSet.end(), FirstVectorSortingAlg);


	points.push_back(dataSet[SIZE_OF_DATA_SET - 1]);
	dataSet.pop_back();
	SIZE_OF_DATA_SET = dataSet.size();
	std::vector<double> pointVector;
	double magnitude = 0;
	std::vector<double> bestPoint;
	double bestVal = 0;
	int bestInd;
	for (int i = 1; i < NUMBER_OF_DIMENSIONS; ++i)
	{
		for (int j = 0; j < SIZE_OF_DATA_SET; ++j)
		{
			for (int k = 0; k < NUMBER_OF_DIMENSIONS; ++k)
			{
				pointVector.push_back(dataSet[j][k] - points[0][k]);
				magnitude += pow(pointVector[pointVector.size()-1],2);
			}
			magnitude = pow(magnitude, 1 / 2);
			
			if (abs(pointVector[i]) / magnitude > bestVal)
			{
				bestPoint = dataSet[j];
				bestVal = pointVector[i] / magnitude;
				bestInd = j;
			}
		}
		points.push_back(bestPoint);
		bestVal = 0;
		dataSet.erase(dataSet.begin() + bestInd);
		SIZE_OF_DATA_SET -= 1;
	}


	std::vector<std::vector<double>> vectors;
	std::vector<double> point = points[0];
	std::vector<double> new_Point;
	if (NUMBER_OF_DIMENSIONS > 1)
	{
		for (int i = 1; i < NUMBER_OF_DIMENSIONS; ++i)
		{
			for (int j = 0; j < NUMBER_OF_DIMENSIONS; ++j)
			{
				new_Point.push_back(points[i][j] - point[j]);
			}
			vectors.push_back(new_Point);
			new_Point.clear();
		}
		orthogonalVector = gaussian_Elimination(vectors);
	}
	else
	{
		orthogonalVector = { 1 };
	}
}

Hyper_Plane::Hyper_Plane(std::vector<std::vector<double>> base_Points, Hyper_Plane base)
{
	basePlane = false;
	points = base_Points;
	const int NUMBER_OF_DIMENSIONS = points[0].size();
	std::vector<std::vector<double>> vectors;
	std::vector<double> point = points[0];
	std::vector<double> new_Point;
	if (NUMBER_OF_DIMENSIONS > 1)
	{
		for (int i = 1; i < NUMBER_OF_DIMENSIONS; ++i)
		{
			for (int j = 0; j < NUMBER_OF_DIMENSIONS; ++j)
			{
				new_Point.push_back(points[i][j] - point[j]);
			}
			vectors.push_back(new_Point);
			new_Point.clear();
		}

		std::vector<double> different_Point;
		std::vector<std::vector<double>> base_Point = base.getPoints();
		bool same = false;
		bool identical = true;
		for (int i = 0; i < base_Points.size(); ++i)
		{
			for (int j = 0; j < base_Point.size(); ++j)
			{
				if (base_Points[j] == base_Point[i])
				{
					same = true;
					break;
				}
			}
			if (!same)
			{
				different_Point = base_Point[i];
				identical = false;
				break;
			}
			same = false;
		}

		orthogonalVector = gaussian_Elimination(vectors);
		
		if (!identical && dist_To_Point(different_Point) < 0)
		{
			flip_Orthogonal_Vector();
		}
		
	}
	else
	{
		orthogonalVector = { 1 };
	}
}

void Hyper_Plane::flip_Orthogonal_Vector()
{
	const int NUMBER_OF_DIMENSIONS = points[0].size();
	int j = 0;
	while (j < NUMBER_OF_DIMENSIONS)
	{
		orthogonalVector[j] *= -1;
		++j;
	}
}

bool Hyper_Plane::check_Point_Outside(std::vector<double> point)
{
	const int NUMBER_OF_DIMENSIONS = points[0].size();
	std::vector<double> point_Vector;
	for (int i = 0; i < NUMBER_OF_DIMENSIONS; ++i)
	{
		point_Vector.push_back(point[i] - points[0][i]);
	}
	double sum = dot(point_Vector);
	if (sum > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Hyper_Plane::is_Point_Outside(std::vector<std::vector<double>> adataSet)
{
	const int SIZE_OF_DATA_SET = adataSet.size();
	int i = 0;
	while (i < SIZE_OF_DATA_SET)
	{
		if (check_Point_Outside(adataSet[i]))
		{
			return true;
		}
		++i;
	}
	return false;
}

double Hyper_Plane::dist_To_Point(std::vector<double> point)
{
	const int NUMBER_OF_DIMENSIONS = orthogonalVector.size();
	double numerator = 0;
	double denominator = 0;

	for (int i = 0; i < NUMBER_OF_DIMENSIONS; ++i)
	{
		numerator += orthogonalVector[i] * ( point[i] - points[0][i]);
		denominator += orthogonalVector[i] * orthogonalVector[i];
	}
	numerator = abs(numerator);
	denominator = sqrt(denominator);
	return numerator / denominator;
}


std::vector<Hyper_Plane> Hyper_Plane::expand_Surface(std::vector<std::vector<double>> adataSet)
{
	const int SIZE_OF_DATA_SET = adataSet.size();
	double maxDist = 0;
	double dist = 0;
	std::vector<double> point;
	std::vector<Hyper_Plane> out;
	// Finds the new point by finding the furthest away point from this plane
	if (is_Point_Outside(adataSet))
	{
		for (int i = 0; i < SIZE_OF_DATA_SET; ++i)
		{
			if (check_Point_Outside(adataSet[i]))
			{
				dist = dist_To_Point(adataSet[i]);
				if (dist > maxDist)
				{
					maxDist = dist;
					point = adataSet[i];
				}
			}
		}
		std::vector<std::vector<double>> outPoints;
		for (int i = 0; i < points.size(); ++i)
        {
			for (int j = 0; j < points.size(); ++j)
			{
				if (i != j)  //Gives every set of n-1 points among current points
				{
					outPoints.push_back(points[j]);
				}
			}
			outPoints.push_back(point);  //Adds the new point
			out.push_back(Hyper_Plane(outPoints, *this));
			outPoints.clear();
		}
		
	}
	else
	{
		out.push_back(*this);
	}
	return out;
}

double Hyper_Plane::dot(std::vector<double> outside)
{
	const int NUMBER_OF_DIMENSIONS = orthogonalVector.size();
	int i = 0;
	double sum = 0;
	while (i < NUMBER_OF_DIMENSIONS)
	{
		sum += outside[i] * orthogonalVector[i];
		++i;
	}
	return sum;
}


Hyper_Plane::~Hyper_Plane()
{}