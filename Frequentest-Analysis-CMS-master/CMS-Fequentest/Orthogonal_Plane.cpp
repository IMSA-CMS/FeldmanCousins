#include "stdafx.h"
#include "Orthogonal_Plane.h"
#include <limits> 
Orthogonal_Plane::Orthogonal_Plane(vector<vector<double>> points)
{
	pointSet = points;
	nDims = pointSet[0].size();
	findStartPoint();
}

double Orthogonal_Plane::Loss(Point testPoint)
{
	double lowestDot = std::numeric_limits<double>::max();
	for (int i = 0; i < pointSet.size(); ++i)
	{
		double d = dot(testPoint.get_Cartesian(), pointSet[i]);
		if (d < lowestDot)
		{
			lowestDot = d;
		}
	}
	return -1*lowestDot;
}

bool Orthogonal_Plane::Check_Plane()
{
	int t = 0;
	const double alpha = 0.001;
	const double beta1 = 0.9;
	const double beta2 = 0.999;
	const double epsilon = 1E-8;
	vector<double> m(nDims-1, 0);
	vector<double> v(nDims-1, 0);
	double oldLoss;
	if (currentLoss < -0.0001)
		return true;
	do
	{
		oldLoss = currentLoss;
		++t;
		auto gradVect = grad();
		m = vector_sum(scalar_product(beta1, m), scalar_product((1 - beta1), gradVect));
		v = vector_sum(scalar_product(beta2, v), scalar_product((1 - beta2), elementwise_product(gradVect,gradVect)));
		auto m_hat = scalar_product(1.0 / (1.0 - pow(beta1, t)), m);
		auto v_hat = scalar_product(1.0 / (1.0 - pow(beta2, t)), v);
		
		auto new_sphere = vector_sum(orthogonalPoint.get_sphere(),
			scalar_product(-1, scalar_product(alpha, elementwise_division(m_hat, elementwise_constant_sum(epsilon, elementwise_sqrt(v_hat))))));
		orthogonalPoint = Point(true, new_sphere);
		currentLoss = Loss(orthogonalPoint);
		if (currentLoss < -0.0001)
			return true;

	} while (abs(oldLoss - currentLoss) > epsilon);

	return false;
}

vector<double> Orthogonal_Plane::grad()
{
	vector<double> gradVect(nDims-1);
	const double rootMachineEpsilon = sqrt(std::numeric_limits<double>::epsilon());
	for (int i = 0; i < nDims-1; ++i)
	{
		auto tempPoint = orthogonalPoint;
		tempPoint.update_sphere(i, orthogonalPoint.get_sphere(i) + rootMachineEpsilon);
		gradVect[i] = (Loss(Point(tempPoint))-currentLoss)/rootMachineEpsilon;
	}
	return gradVect;
}

void Orthogonal_Plane::findStartPoint()
{
	double bestLoss = 1000000;
	const double epsilon = 0.0001;
	vector<double> bestPoint;
	for (int i = 0; i < nDims; ++i)
	{
		vector<double> testPoint(nDims,epsilon);
		testPoint[i] = 1;
		double currentL = Loss(Point(testPoint));
		if (currentLoss < -0.0001)
			break;
		if (currentL < bestLoss)
		{
			bestLoss = currentL;
			bestPoint = testPoint;
		}
		testPoint[i] = -1;
		currentL = Loss(Point(testPoint));
		if (currentLoss < -0.0001)
			break;
		if (currentL < bestLoss)
		{
			bestLoss = currentL;
			bestPoint = testPoint;
		}
		testPoint[i] = 0;
	}
	currentLoss = bestLoss;
	orthogonalPoint = bestPoint;
}


