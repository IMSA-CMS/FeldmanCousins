#include "stdafx.h"
#include <algorithm>
#include <set>
#include "Point.h"
#include "Matrix.cpp"
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
	if (cartesian.size() <= 1)
		return;
	cartesian = vector<double>(spherical.size()+1, 0);
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

void Point::rotate_theta(Point p)
{
	spherical.back() -= p.get_theta();
	update_cartesian();
}

void Point::rotate_phi(double angle, int index)
{
	std::vector<std::vector<double>> rotation_mat (spherical.size());
	for (int i = 0; i < spherical.size(); ++i) //ERROR: PRODUCES N-1 SQUARE INSTEAD OF N
	{
		for (int j = 0; j < spherical.size(); ++j)
		{
			if (i == 0 && j == 0)
			{
				rotation_mat[i].push_back(cos(angle));
			}
			else if (i == 0 && j != index)
			{
				rotation_mat[i].push_back(0);
			}
			else if (i == 0 && j == index)
			{
				rotation_mat[i].push_back(-sin(angle));
			}

			else if (i == index && j == 0)
			{
				rotation_mat[i].push_back(sin(angle));
			}

			else if (i == index && j != index)
			{
				rotation_mat[i].push_back(0);
			}

			else if (i == index && j == index)
			{
				rotation_mat[i].push_back(cos(angle));
			}

			else if (i != j)
			{
				rotation_mat[i].push_back(0);
			}

			else if (i == j)
			{
				rotation_mat[i].push_back(1);
			}
			else
			{
				throw "This shouldn't happen";
			}
		}
	}
	std::vector<double> new_cartesian;
	double product;
	for (int i = 0; i < rotation_mat.size(); ++i)
	{
		product = 0;
		for (int j = 0; j < rotation_mat[0].size(); ++j)
		{
			product += rotation_mat[i][j] * cartesian[j];
		}
		new_cartesian.push_back(product);
	}
	cartesian = new_cartesian;
}
void Point::popDim(int index)
{
	std::vector<double> newPt;
	for (int i = 0; i < cartesian.size(); ++i)
	{
		if (i != index)
		{
			newPt.push_back(cartesian[i]);
		}
	}
	cartesian = newPt;
	update_sphere();
}