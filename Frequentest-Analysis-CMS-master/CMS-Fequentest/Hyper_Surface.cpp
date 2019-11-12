#include "stdafx.h"
#include "Hyper_Surface.h"


Hyper_Surface::Hyper_Surface(std::vector<std::vector<double>*> adataSet)
{
	dataSet = adataSet;
	basePlane = Hyper_Plane(dataSet);
	basePlane.flip_Orthogonal_Vector();
	basePlane.clear_Out();
	basePlane.add_Out(adataSet);
}

bool Hyper_Surface::check_If_More()
{
	int NUMBER_OF_PLANES = planeSet.size();
	const int NUMBER_OF_DATA_POINTS = dataSet.size();
	for (int i = 0; i < NUMBER_OF_PLANES; ++i)
	{
		if (planeSet[i].getOutPoints().size() != 0)
		{
			return true;
		}
	}
	return false;
}

void Hyper_Surface::build(Hyper_Plane plane)
{
	auto far_point = plane.find_far();
	std::vector<Hyper_Plane> vis_planes;
	for (unsigned i = 0; i < planeSet.size(); ++i)
	{
		auto out_points = planeSet[i].getOutPoints();
		for (unsigned j = 0; j < out_points.size(); ++j)
		{
			if (far_point == out_points[j])
			{
				vis_planes.push_back(planeSet[i]);
				planeSet.erase(planeSet.begin() + i);
			}
		}
	}

	for (unsigned i = 0; i < vis_planes.size(); ++i)
	{
		auto temp = vis_planes[i].expand_Surface(far_point);
		for (unsigned j = 0; j < temp.size(); ++j)
		{
			planeSet.push_back(temp[j]);
		}
	}
	clear_Planes();
	clear_Points();
}


void Hyper_Surface::make_Surface()
{
	planeSet.push_back(Hyper_Plane(dataSet));
	dataSet.clear();
	std::vector<Hyper_Plane> tempSet;
	while (check_If_More())
	{
		for (unsigned i = 0; i < planeSet.size(); ++i)
		{
			if (planeSet[i].getOutPoints().size() > 0)
			{
				build(planeSet[i]);
				break;
			}
		}
	}
}

bool Hyper_Surface::point_In_Base(std::vector<double>* point)
{
	if (basePlane.check_Point_Outside(point))
	{
		return false;
	}
	return true;
}

bool Hyper_Surface::point_Is_In(std::vector<double>* point)
{

	int NUMBER_OF_PLANES = planeSet.size();
	if (basePlane.check_Point_Outside(point))
		return false;

	for (int i = 0; i < NUMBER_OF_PLANES; ++i)
	{
		if (planeSet[i].check_Point_Outside(point))
			return false;
	}
	return true;
}


void Hyper_Surface::clear_Planes()
{
	std::vector<Hyper_Plane> new_set;
	new_set.push_back(planeSet[0]);
	auto same = false;
	for (unsigned i = 1; i < planeSet.size(); ++i)
	{
		for (unsigned j = 0; j < new_set.size(); ++j)
		{
			if (planeSet[i] == new_set[j])
				same = true;
		}
		if (!same)
			new_set.push_back(planeSet[i]);
	}
	planeSet = new_set;
}

void Hyper_Surface::clear_Points()
{
	int NUMBER_OF_PLANES = planeSet.size();
	int NUMBER_OF_DATA_POINTS = dataSet.size();
	int NUMBER_OF_DIMENSIONS = dataSet[0]->size();
	std::vector<std::vector<double>*> newSet;
	std::vector<Hyper_Plane> newPlaneSet;
	for (int i = NUMBER_OF_PLANES - 1; i >= 0; --i)
	{
		bool bad = false;
		auto ortho = planeSet[i].getOrtho();
		for (int j = 0; j < NUMBER_OF_DIMENSIONS; ++j)
		{
			if (isnan(ortho[j]))
				bad = true;
		}
		if (!bad)
			newPlaneSet.push_back(planeSet[i]);
	}
	planeSet = newPlaneSet;
	newPlaneSet.clear();
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
