#pragma once
#include "CMS-Frequentest-Analysis.h"
class Plane
{

public:
	Plane(std::vector<std::vector<double>>& shifted_data);

	// Takes the dot product of the plane and vect
	double dot(std::vector<double> vect);

	// Finds the cosine between the plane and vect
	double cosine_theta(std::vector<double> vect);
	std::vector<double> get_orthogonal() { return orthogonal; }

private:
	std::vector<std::vector<double>> points;
	std::vector<double> orthogonal;

};

