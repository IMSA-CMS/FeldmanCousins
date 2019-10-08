#include "stdafx.h"
#include "Hyper_Surface.h"


Hyper_Surface::Hyper_Surface(std::vector<std::vector<double>> adataSet)
{
	dataSet = adataSet;
	basePlane = Hyper_Plane(dataSet);
	basePlane.flip_Orthogonal_Vector();
}

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
	std::vector<std::vector<Hyper_Plane>> tempSet;
	while (check_If_More())
	{
		for (int i = 0; i < planeSet.size(); ++i)
		{
			tempSet.push_back(planeSet[i].expand_Surface(dataSet));
		}
		planeSet.clear();
		for (int j = 0; j < tempSet.size(); ++j)
		{
			for (int k = 0; k < tempSet[j].size(); ++k)
			{
				planeSet.push_back(tempSet[j][k]);
			}
		}
		tempSet.clear();
		clear_Points();
	}
}

bool Hyper_Surface::point_In_Base(std::vector<double> point)
{
	if (basePlane.check_Point_Outside(point))
	{
		return false;
	}
	return true;
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
