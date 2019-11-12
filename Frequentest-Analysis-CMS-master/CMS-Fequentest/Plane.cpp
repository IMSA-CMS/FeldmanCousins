#include "stdafx.h"
#include <algorithm>
#include "Plane.h"

Plane::Plane(std::vector<std::vector<double>>& data)
{
	const int DIMENSIONS = data[0].size();
	std::vector<double> origin(DIMENSIONS,0.0);
	
	points.push_back(origin);
	for (unsigned int i = 0; i < DIMENSIONS - 1; ++i)
	{
		orthogonal.push_back(0);
	}
	orthogonal.push_back(1);
	int bestInd;
	std::vector<double> bestPoint;

	for (int i = 1; i < DIMENSIONS; ++i)
	{
		double maxCos = 0;
		for (int j = 0; j < data.size(); ++j)
		{
			double cosine = cosine_theta(data[j]);
			if (cosine > maxCos)
			{
				maxCos = cosine;
				bestInd = i;
			}
		}
		points.push_back(data[bestInd]);
		orthogonal = gaussian_Elimination(points);
		data.erase(data.begin() + bestInd);
	}


	orthogonal = gaussian_Elimination(points);
	if (DIMENSIONS == 1)
	{
		orthogonal = { 1 };
	}
}

double Plane::dot(std::vector<double> vect)
{
	double sum = 0;
	for (unsigned int i = 0; i < orthogonal.size(); ++i)
	{
		sum += orthogonal[i] * vect[i];
	}
	return sum;
}

double Plane::cosine_theta(std::vector<double> vect)
{
	return dot(vect) / (magnitude(vect) * magnitude(orthogonal));
}

