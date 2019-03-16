#include "stdafx.h"
#include "Hyper_Surface.h"


Hyper_Surface::Hyper_Surface(std::vector<std::vector<float>> adataSet):
	dataSet(adataSet)
{}

bool Hyper_Surface::check_If_More()
{	
	for (std::size_t i = 0; i < NUMBER_OF_PLANES; ++i)
	{
		for (std::size_t j = 0; j < NUMBER_OF_DATA_POINTS; ++j)
		{
			if (planeSet[i].check_Point_Outside(dataSet[j]))
			{
				return true;
			}
		}
		
	}
	return false;
}

void Hyper_Surface::make_Surface()
{
	planeSet.push_back(Hyper_Plane(dataSet));
	std::vector<std::vector<Hyper_Plane>> tempSet;
	while (check_If_More())
	{
		for (std::size_t i = 0; i < planeSet.size(); ++i)
		{
			tempSet = planeSet[i].expand_Surface;
		}
		planeSet.clear();
		for (std::size_t j = 0; j < tempSet.size(); ++j)
		{
			for (std::size_t k = 0; k < tempSet[0].size(); ++k)
			{
				planeSet.push_back(tempSet[j][k]);
			}
		}
		clear_Points();
	}
}

bool Hyper_Surface::point_Is_In(std::vector<float> point)
{
	bool inside = true;
	for (std::size_t i = 0; i < NUMBER_OF_PLANES; ++i)
	{
		if (planeSet[i].check_Point_Outside(point))
		{
			inside = false;
		}
	}
	return inside;
}

void Hyper_Surface::clear_Points()
{
	const int NUMBER_OF_DATA_POINTS = dataSet.size();
	std::vector<std::vector<float>> newSet;
	for (std::size_t i = 0; i < NUMBER_OF_DATA_POINTS; ++i)
	{
		if (!point_Is_In(dataSet[i]))
		{
			newSet.push_back(dataSet[i]);
		}
	}
	dataSet = newSet;
	newSet.clear();
}

Hyper_Surface::~Hyper_Surface()
{
}
