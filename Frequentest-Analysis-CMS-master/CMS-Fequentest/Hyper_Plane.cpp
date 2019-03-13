#include "stdafx.h"
#include "Hyper_Plane.h"
#include "CMS-Frequentest-Analysis.h"


Hyper_Plane::Hyper_Plane(std::vector<std::vector<float>>& adataSet)
{
	basePlane = true;
	const int SIZE_OF_DATA_SET = adataSet.size();
	const int NUMBER_OF_DIMENSIONS = adataSet[0].size();
	dataSet = adataSet;
	for (std::size_t i = 0; i < NUMBER_OF_DIMENSIONS; ++i)
	{
		points.push_back(find_Smallest_By_Index(adataSet, i));
	}
	std::vector<float> orthogonalVector = guassian_Elimination(points);
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

Hyper_Plane::Hyper_Plane(std::vector<std::vector<float>>& adataSet,
	std::vector<std::vector<float>> base_Points, Hyper_Plane base)
{
	basePlane = false;
	dataSet = adataSet;
	points = base_Points;
	const int SIZE_OF_DATA_SET = adataSet.size();
	const int NUMBER_OF_DIMENSIONS = adataSet[0].size();
	std::vector<float> ortho = guassian_Elimination(points);
	orthogonalVector = make_Unit_Vector(ortho);
	if (find_Smallest_By_Index(base_Points, 0)[0] < base.smallest_Point()[0])
	{
		if (orthogonalVector[0] > 0)
		{
			for (std::size_t i = 0; i < NUMBER_OF_DIMENSIONS; ++i)
			{
				orthogonalVector[i] *= -1;
			}
		}
	}
	else    // add an else if for the case of equal
	{
		if (orthogonalVector[0] < 0)
		{
			for (std::size_t i = 0; i < NUMBER_OF_DIMENSIONS; ++i)
			{
				orthogonalVector[i] *= -1;
			}
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

std::vector<float> Hyper_Plane::smallest_Point()
{
	return find_Smallest_By_Index(points, 0);
}


bool Hyper_Plane::check_Point_Outside(std::vector<float> point)
{
	const int NUMBER_OF_DIMENSIONS = dataSet[0].size();
	float sum = 0;
	for (std::size_t i; i < NUMBER_OF_DIMENSIONS; ++i)
	{
		sum += point[i] * orthogonalVector[i];
	}
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

double Hyper_Plane::dist_To_Point(std::vector<float> point)
{
	return 1;
}

std::vector<Hyper_Plane> Hyper_Plane::expand_Surface()
{
	const int SIZE_OF_DATA_SET = dataSet.size();
	float dist = 0;
	std::vector<float> point;
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
	// Run through every permunation of points of size points.size()-1 and make a vector of Hyper_Plane with 
	// Hyper_Plane of that permutation of points, this base and referenct to dataSet
}

Hyper_Plane::~Hyper_Plane()
{
}
