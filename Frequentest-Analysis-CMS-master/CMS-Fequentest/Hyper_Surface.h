#pragma once
#include <vector>
#include "Hyper_Plane.h"
class Hyper_Surface
{
private:
	std::vector<std::vector<double>> dataSet;
	std::vector<Hyper_Plane> planeSet;
	
	
public:
	Hyper_Surface(std::vector<std::vector<double>> adataSet);
	~Hyper_Surface();
	// Checks if there are more points outside of the surface
	bool check_If_More();
	// Makes the whole Hyper_Surface
	void make_Surface();
	// Checks if a point is in the surface
	bool point_Is_In(std::vector<double> point);
	// Cleares all the points inside of the Hyper_Surface
	void clear_Points();
};

