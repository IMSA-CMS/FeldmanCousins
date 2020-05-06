#pragma once
#include "CMS-Frequentest-Analysis.h"
#include "Matrix.cpp"
class Point
{
public:
	Point(std::vector<double> point);
	//converts to n-dimensional spherical coordinates in the form (phi1,phi2...phi(n-1),theta);
	void update_sphere();
	void update_cartesian();
	//Rotates this point by same amount as it would take to rotate p to theta = 0
	void rotate_theta(Point p);
	//Rotates point at index in the phi direction perpendiculare to 
	void rotate_phi(double angle, int index);
	double get_theta() { return spherical.back(); }
	double get_sphere(int index) { return spherical[index]; }
	int get_dim() { return cartesian.size(); }
	//Removes the value at index
	void popDim(int index);
private:
	std::vector<double> cartesian;
	//in the form (phi_1,phi_2,phi_3...phi_(n-1),theta) where n is the number of dimesions
	std::vector<double> spherical;

};

