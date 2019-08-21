#include "stdafx.h"
#include "Hyper_Plane.h"
#include "CMS-Frequentest-Analysis.h"


Hyper_Plane::Hyper_Plane(std::vector<std::vector<double>>& adataSet) //Functional
{
	basePlane = true;
	const int SIZE_OF_DATA_SET = adataSet.size();
	const int NUMBER_OF_DIMENSIONS = adataSet[0].size();
	dataSet = adataSet;
	for (int i = 0; i < NUMBER_OF_DIMENSIONS; ++i)
	{
		points.push_back(dataSet[SIZE_OF_DATA_SET - 1 - i]);   //Having the largest n points in a dimension guarantees a plane on the outside 
	}
	std::vector<std::vector<double>> vectors;
	std::vector<double> point = points[0];
	std::vector<double> new_Point;
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
	bool org;
	for (int i = 0; i < points.size(); ++i)
	{
		org = true;
		for (int j = 0; j < NUMBER_OF_DIMENSIONS; ++j)
		{
			if (points[i][j] != 0)
			{
				org = false;
				break;
			}
		}
		if (org)
		{
			break;
		}
	}
	if (org)
	{
		origin = true;
	}
	else
	{
		origin = false;
	}
	std::vector<double> last = dataSet[dataSet.size()-1];
	if (dot(last) > 0)
	{
		for (int i = 0; i < orthogonalVector.size(); ++i)
		{
			orthogonalVector[i] *= -1;
		}
	}
}

Hyper_Plane::Hyper_Plane(std::vector<std::vector<double>>& adataSet,
	std::vector<std::vector<double>> base_Points, Hyper_Plane base) //Functional
{
	basePlane = false;
	dataSet = adataSet;
	points = base_Points;
	const int SIZE_OF_DATA_SET = adataSet.size();
	const int NUMBER_OF_DIMENSIONS = adataSet[0].size();
	std::vector<std::vector<double>> vectors;
	std::vector<double> point = points[0];
	std::vector<double> new_Point;
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
	for (int i = 0; i < base_Points.size(); ++i)
	{
		for (int j = 0; j < base_Point.size(); ++j)
		{
			if (base_Points[i] == base_Point[j])
			{
				same = true;
				break;
			}
		}
		if (!same)
		{
			different_Point = base_Points[i];
			break;
		}
		same = false;
	}

	orthogonalVector = gaussian_Elimination(vectors);

	if (dist_To_Point(different_Point) > 0)
	{
		int j = 0;
		while (j < NUMBER_OF_DIMENSIONS)
		{
			orthogonalVector[j] *= -1;
			++j;
		}
	}
	bool org;
	for (int i = 0; i < points.size(); ++i)
	{
		org = true;
		for (int j = 0; j < NUMBER_OF_DIMENSIONS; ++j)
		{
			if (points[i][j] != 0)
			{
				org = false;
				break;
			}
		}
		if (org)
		{
			break;
		}
	}
	if (org)
	{
		origin = true;
	}
	else
	{
		origin = false;
	}
}



bool Hyper_Plane::check_Point_Outside(std::vector<double> point) //Functional
{
	const int NUMBER_OF_DIMENSIONS = dataSet[0].size();
	std::vector<double> point_Vector;
	for (int i = 0; i < NUMBER_OF_DIMENSIONS; ++i)
	{
		point_Vector.push_back(point[i] - points[0][i]);
	}
	double sum = dot(point_Vector);
	if (sum < 0)
	{
		return true;
	}
	else
	{
		return false;
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

double Hyper_Plane::dist_To_Point(std::vector<double> point) //Functional
{
	const int NUMBER_OF_DIMENSIONS = orthogonalVector.size();
	double numerator = 0;
	double denominator = 0;
	for (int i = 0; i < NUMBER_OF_DIMENSIONS; ++i)
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
		for (int i = 0; i < SIZE_OF_DATA_SET; ++i)
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
			out.push_back(Hyper_Plane(dataSet, outPoints, *this));
			outPoints.clear();
		}
		
	}
	else
	{
		out.push_back(*this);
	}
	return out;
}

double Hyper_Plane::dot(std::vector<double> outside) //Functional
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