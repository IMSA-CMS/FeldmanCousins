#pragma once
#include <vector>
#include "Hyper_Plane.h"
class Hyper_Surface
{
private:
	std::vector<std::vector<double>*> dataSet;
	Hyper_Plane basePlane;
	std::vector<Hyper_Plane> planeSet;
	
	
public:
	Hyper_Surface(std::vector<std::vector<double>*> adataSet);

	// Checks if there are more points outside of the surface
	bool check_If_More();

	// Builds one step of Quick Hull
	void build(Hyper_Plane plane);

	// Makes the whole Hyper_Surface
	void make_Surface();

	// Checks if a point is in the surface
	bool point_Is_In(std::vector<double>* point);

	// Checks if a point is in the right side of the base
	bool point_In_Base(std::vector<double>* point);

	// Clears created duplicate planes
	void clear_Planes();

	// Cleares all the points inside of the Hyper_Surface
	void clear_Points();

	~Hyper_Surface() { ; }
};

