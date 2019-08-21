#include "stdafx.h"
#include "Hyper_Surface.h"


Hyper_Surface::Hyper_Surface(std::vector<std::vector<double>> adataSet):
	dataSet(adataSet)
{}

bool Hyper_Surface::check_If_More()
{	
	int NUMBER_OF_PLANES = planeSet.size();
	const int NUMBER_OF_DATA_POINTS = dataSet.size();
	for (int i = 0; i < NUMBER_OF_PLANES; ++i)
	{
		for (int j = 0; j < NUMBER_OF_DATA_POINTS; ++j)
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
	Hyper_Plane basePlane = planeSet[0];
	std::vector<std::vector<Hyper_Plane>> tempSet;
	while (check_If_More())
	{
		for (int i = 0; i < planeSet.size(); ++i)
		{
			tempSet.push_back(planeSet[i].expand_Surface());
		}
		planeSet.clear();
		for (int j = 0; j < tempSet.size(); ++j)
		{
			for (int k = 0; k < tempSet[0].size(); ++k)
			{
				planeSet.push_back(tempSet[j][k]);
			}
		}
		tempSet.clear();
		clear_Points();
	}
	planeSet.push_back(basePlane);
}

bool Hyper_Surface::point_Is_In(std::vector<double> point)
{

	int NUMBER_OF_PLANES = planeSet.size();
	for (int i = 0; i < NUMBER_OF_PLANES; ++i)
	{
		if (planeSet[i].check_Point_Outside(point))
		{
			return false;
		}
	}
	return true;
}

void Hyper_Surface::clear_Points()
{
	int NUMBER_OF_DATA_POINTS = dataSet.size();
	std::vector<std::vector<double>> newSet;
	for (int i = NUMBER_OF_DATA_POINTS-1; i >=0; --i)
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
