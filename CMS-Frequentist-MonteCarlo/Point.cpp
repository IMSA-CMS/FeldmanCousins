#include "stdafx.h"
#include <algorithm>
#include <set>
#include "Point.h"
#include <cmath>  
#include "Constants.h"
Point::Point()
{
	cartesian = std::vector<double>(1, 0);
	spherical = std::vector<double>(1, 0);
}
Point::Point(std::vector<double> point)
{
	cartesian = point;
	update_sphere();
	update_cartesian();
}

Point::Point(bool isSphere, std::vector<double> point)
{
	if (isSphere)
	{
		spherical = point;
		update_cartesian();
	}
	else
	{
		cartesian = point;
		update_sphere();
		update_cartesian();
	}
}

void Point::update_sphere()
{
	if (cartesian.size() == 1)
		return;
	std::vector<double> sphere;
	for (int i = 0; i < cartesian.size() - 2; ++i)
	{
		double sum = 0;
		for (int j = cartesian.size()-1; j >= i; --j)
		{
			sum += cartesian[j] * cartesian[j];
		}
		double frac = cartesian[i] / sqrt(sum);
		sphere.push_back(acos(frac));
	}
	if (cartesian[cartesian.size() - 1] >= 0)
	{
		double theta = acos(cartesian[cartesian.size() - 2] / sqrt(cartesian[cartesian.size() - 1] * cartesian[cartesian.size() - 1] + cartesian[cartesian.size() - 2] * cartesian[cartesian.size() - 2]));
		sphere.push_back(theta);
	}
	else
	{
		double theta = 2* Constants::pi - acos(cartesian[cartesian.size() - 2] / sqrt(cartesian[cartesian.size() - 1] * cartesian[cartesian.size() - 1] + cartesian[cartesian.size() - 2] * cartesian[cartesian.size() - 2]));
		sphere.push_back(theta);
	}
	spherical = sphere;
}

void Point::update_sphere(int index, double value)
{
	spherical[index] = value;
	update_cartesian();
}

void Point::update_cartesian()
{
	if (cartesian.size() <= 1 && spherical.size()==0)
		return;
	cartesian = std::vector<double>(spherical.size()+1, 0);
	cartesian[0] = cos(spherical[0]);
	for (int i = 1; i < spherical.size(); ++i)
	{
		double product = cos(spherical[i]);
		for (int j = 0; j < i; ++j)
		{
			product *= sin(spherical[j]);
		}
		cartesian[i] = product;
	}
	double product = 1;
	for (int i = 0; i < spherical.size(); ++i)
	{
		product *= sin(spherical[i]);
	}
	cartesian[spherical.size()] = product;
}


