#include "stdafx.h"
#include "Hyper_Plane.h"
#include "CMS-Frequentest-Analysis.h"



Hyper_Plane::Hyper_Plane(std::vector<std::vector<float>>& adataSet)
{
	const int LENGTH_OF_DATA_SET = adataSet.size();
	const int NUMBER_OF_DIMENSIONS = adataSet[0].size();
	dataSet = adataSet;
	for (std::size_t i = 0; i < NUMBER_OF_DIMENSIONS; ++i)
	{
		points.push_back(find_Smallest_By_Index(adataSet, i));
	}
	

}

Hyper_Plane::Hyper_Plane(std::vector<std::vector<float>>& adataSet, std::vector<std::vector<float>> base_Points, Hyper_Plane base)
{

}

bool Hyper_Plane::check_Point_Outside()
{

}

std::vector<Hyper_Plane> Hyper_Plane::expand_Surface()
{

}

Hyper_Plane::~Hyper_Plane()
{
}
