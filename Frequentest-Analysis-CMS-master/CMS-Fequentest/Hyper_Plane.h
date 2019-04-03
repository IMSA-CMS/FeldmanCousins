#pragma once
#include <vector>

class Hyper_Plane
{
private:
	std::vector<std::vector<float>> dataSet;
	std::vector<std::vector<float>> points;
	std::vector<double> orthogonalVector;
	bool basePlane;
	bool origin;
public:
	//Creates the base Hyper_Plane
	Hyper_Plane(std::vector<std::vector<float>>& adataSet);
	//Creates a Hyper_Plane from another Hyper_Plane
	Hyper_Plane(std::vector<std::vector<float>>& adataSet, std::vector<std::vector<float>> base_Points, Hyper_Plane base);
	//Finds the distance between the Hyper_Plane and the point
	double dist_To_Point(std::vector<float> point);
	//Checks if a point is outside of the current plane
	bool check_Point_Outside(std::vector<float> point);
	//Constructs a vector of hyperplanes taking every combination of base points and a new point with largest orthogonal distance
	std::vector<Hyper_Plane> expand_Surface();
	//Finds the point with the smallest x value in the Hyper_Plane
	std::vector<float> smallest_Point();
	~Hyper_Plane();
};
