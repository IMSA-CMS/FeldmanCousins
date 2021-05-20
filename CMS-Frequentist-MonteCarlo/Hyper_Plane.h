#ifndef CMS_FREQUENTIST_MONTECARLO_HYPERPLANE_H
#define CMS_FREQUENTIST_MONTECARLO_HYPERPLANE_H
#include <vector>



class Hyper_Plane
{
private:
	std::vector<std::vector<double>*> outPoints;
	std::vector<std::vector<double>*> points;
	std::vector<double> orthogonalVector;
	bool basePlane;

public:
	//Default Constructor for Hyper_Plane
	Hyper_Plane() {};

	//Creates the base Hyper_Plane
	Hyper_Plane(std::vector<std::vector<double>*> adataSet);


	bool operator == (Hyper_Plane const& rhs)
	{
		if (points == rhs.points)
			return true;
		return false;
	}

	bool operator != (Hyper_Plane const& rhs)
	{
		if (points != rhs.points)
			return true;
		return false;
	}

	Hyper_Plane(const Hyper_Plane& h)
	{
		outPoints = h.outPoints;
		points = h.points;
		orthogonalVector = h.orthogonalVector;
		basePlane = h.basePlane;
	}

	//Creates a Hyper_Plane from another Hyper_Plane
	Hyper_Plane(std::vector<std::vector<double>*> base_Points, Hyper_Plane base);

	//Gets points
	std::vector<std::vector<double>*> getPoints() { return points; }

	//Gets Orthogonal Vector
	std::vector<double> getOrtho() { return orthogonalVector; }

	//Gets outPoints
	std::vector<std::vector<double>*> getOutPoints() { return outPoints; }

	//Flips the orthogonal vector
	void flip_Orthogonal_Vector();

	//Finds the distance between the Hyper_Plane and the point
	double dist_To_Point(std::vector<double>* point);

	//Checks if a point is outside of the current plane
	bool check_Point_Outside(std::vector<double>* point);

	//Finds all points outside of the plane in the dataSet and adds them to outPoints
	void add_Out(std::vector<std::vector<double>*> adataSet);

	//Adds a point to outPoints
	void add_to_out(std::vector<double>* point);
	
	//Clears outPoints
	void clear_Out() { outPoints.clear(); }

	//Checks if there are any points in the right direction of the plane
	bool is_Point_Outside(std::vector<std::vector<double>*> adataSet);

	//Finds the furthest point from the plane
	std::vector<double>* find_far();

	//Constructs a vector of hyperplanes taking every combination of base points and a given new point
	std::vector<Hyper_Plane> expand_Surface(std::vector<double>* point);

	//Finds the dot product between the orthogonal vector and another vector
	double dot(std::vector<double>* outside);



};
#endif
