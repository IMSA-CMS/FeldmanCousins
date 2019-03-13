#pragma once
#include <vector>
#include <random>

class Hyper_Plane
{
private:
	std::vector<std::vector<float>> dataSet;
	std::vector<std::vector<float>> points;
	std::vector<double> orthogonalVector;

public:
	Hyper_Plane(std::vector<std::vector<float>>& adataSet);
	Hyper_Plane(std::vector<std::vector<float>>& adataSet, std::vector<std::vector<float>> base_Points, Hyper_Plane base);
	//Checks if a point is outside of the current plane
	bool check_Point_Outside(); 
	//Constructs a vector of hyperplanes taking every combination of base points and a new point with largest orthogonal distance
	std::vector<Hyper_Plane> expand_Surface();
	~Hyper_Plane();
};

