#include "stdafx.h"
#include "Hyper_Plane.h"
#include "CMS-Frequentest-Analysis.h"


Hyper_Plane::Hyper_Plane(std::vector<std::vector<double>>& adataSet)
{
	basePlane = true;
	const int SIZE_OF_DATA_SET = adataSet.size();
	const int NUMBER_OF_DIMENSIONS = adataSet[0].size();
	dataSet = adataSet;
	bubSort(dataSet, 0);
	for (std::size_t i = 0; i < NUMBER_OF_DIMENSIONS; ++i)
	{
		points.push_back(dataSet[i]);
	}
	
	


	std::vector<std::vector<double>> vectors;
	std::vector<double> point = points[0];
	std::vector<double> new_Point;
	for (std::size_t i = 1; i < NUMBER_OF_DIMENSIONS; ++i)
	{
		for (std::size_t j = 0; j < NUMBER_OF_DIMENSIONS; ++i)
		{
			new_Point.push_back(points[i][j] - point[j]);
		}
		vectors.push_back(new_Point);
		new_Point.clear();
	}
	orthogonalVector = gaussian_Elimination(vectors);
	bool org;
	for (std::size_t i = 0; i < SIZE_OF_DATA_SET; ++i)
	{
		org = true;
		for (std::size_t j = 0; j < NUMBER_OF_DIMENSIONS; ++j)
		{
			if (adataSet[i][j] != 0)
			{
				org = false;
			}
		}
		if (org)
		{
			origin = true;
			break;
		}
	}
	if (org != true)
	{
		origin = false;
	}

}

Hyper_Plane::Hyper_Plane(std::vector<std::vector<double>>& adataSet,
	std::vector<std::vector<double>> base_Points, Hyper_Plane base)
{
	basePlane = false;
	dataSet = adataSet;
	points = base_Points;
	const int SIZE_OF_DATA_SET = adataSet.size();
	const int NUMBER_OF_DIMENSIONS = adataSet[0].size();
	std::vector<double> orthogonalVector = gaussian_Elimination(points);
	if (base.dot(orthogonalVector) < 0)
	{
		int j = 0;
		while (j < NUMBER_OF_DIMENSIONS)
		{
			orthogonalVector[j] *= -1;
			++j;
		}
	}
	bool org;
	for (std::size_t i = 0; i < SIZE_OF_DATA_SET; ++i)
	{
		org = true;
		for (std::size_t j = 0; j < NUMBER_OF_DIMENSIONS; ++j)
		{
			if (adataSet[i][j] != 0)
			{
				org = false;
			}
		}
		if (org)
		{
			origin = true;
		}
	}
	if (org != true)
	{
		origin = false;
	}
}

std::vector<double> Hyper_Plane::smallest_Point()
{
	return find_Smallest_By_Index(points, 0);
}


bool Hyper_Plane::check_Point_Outside(std::vector<double> point)
{
	const int NUMBER_OF_DIMENSIONS = dataSet[0].size();
	double sum = dot(point);
	if (origin)
	{
		if (sum > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (sum > 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool Hyper_Plane::is_Point_Outside()
{
	const int SIZE_OF_DATA_SET = dataSet.size();
	int i = 0;
	while (i < SIZE_OF_DATA_SET)
	{
		if (check_Point_Outside(dataSet[i]))
		{
			return true;
		}
	}
	return false;
}

double Hyper_Plane::dist_To_Point(std::vector<double> point)
{
	const int NUMBER_OF_DIMENSIONS = orthogonalVector.size();
	double numerator = 0;
	double denominator = 0;
	for (std::size_t i = 0; i < NUMBER_OF_DIMENSIONS; ++i)
	{
		numerator += orthogonalVector[i] * point[i];
		denominator += pow(orthogonalVector[i], 2);
	}
	if (!origin)  //If the origin is a part of this plane the constant term is 0 and all terms are already included
	{
		numerator -= 1;    //Equation is in the form Ax_1+bx_2...=1 so when put in the equation of the form Ax_1+Bx_2...+N=0 we get that N=-1
	}
	numerator = abs(numerator);
	denominator = sqrt(denominator);
	return numerator / denominator;
}


std::vector<Hyper_Plane> Hyper_Plane::expand_Surface()
{
	const int SIZE_OF_DATA_SET = dataSet.size();
	double dist = 0;
	std::vector<double> point;
	std::vector<Hyper_Plane> out;
	// Finds the new point by finding the furthest away point from this plane
	if (is_Point_Outside())
	{
		for (std::size_t i = 0; i < SIZE_OF_DATA_SET; ++i)
		{
			if (check_Point_Outside(dataSet[i]))
			{
				if (dist_To_Point(dataSet[i]) > dist)
				{
					dist = dist_To_Point(dataSet[i]);
					point = dataSet[i];
				}
			}
		}
		std::vector<std::vector<double>> outPoints;
		for (std::size_t i = 0; i < points.size(); ++i)
		{
			for (std::size_t j = 0; j < points.size(); ++j)
			{
				if (i != j)  //Gives every set of n-1 points among current points
				{
					outPoints.push_back(points[j]);
				}
			}
			outPoints.push_back(point);  //Adds the new point
			out.push_back(Hyper_Plane(dataSet, outPoints, *this));
			std::vector<std::vector<double>> outPoints;
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
		sum += i * orthogonalVector[i];
		++i;
	}
	return sum;
}

Hyper_Plane::~Hyper_Plane()
{}