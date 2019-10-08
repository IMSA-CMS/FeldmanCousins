#pragma once
#include <vector>



class Hyper_Plane
{
private:
	std::vector<std::vector<double>> points;
	std::vector<double> orthogonalVector;
	bool basePlane;

public:
	//Default Constructor for Hyper_Plane
	Hyper_Plane() {};

	//Creates the base Hyper_Plane
	Hyper_Plane(std::vector<std::vector<double>> adataSet);

	//Creates a Hyper_Plane from another Hyper_Plane
	Hyper_Plane(std::vector<std::vector<double>> base_Points, Hyper_Plane base);

	//Gets points set
	std::vector<std::vector<double>> getPoints() { return points; }

	//Flips the orthogonal vector
	void flip_Orthogonal_Vector();

	//Finds the distance between the Hyper_Plane and the point
	double dist_To_Point(std::vector<double> point);

	//Checks if a point is outside of the current plane
	bool check_Point_Outside(std::vector<double> point);

	//Checks if there are any points in the right direction of the plane
	bool is_Point_Outside(std::vector<std::vector<double>> adataSet);

	//Constructs a vector of hyperplanes taking every combination of base points and a new point with largest orthogonal distance
	std::vector<Hyper_Plane> expand_Surface(std::vector<std::vector<double>> adataSet);

	//Finds the dot product between the orthogonal vector and another vector
	double dot(std::vector<double> outside);


	~Hyper_Plane();
};

