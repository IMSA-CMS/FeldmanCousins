#pragma once
#include "Bin.h"
//Interval.h: Represents an interval with a lower and upper bound. Used for bounds checking.
template<typename T>
class Interval
{
	public:
		//Constructs Interval with a lower and upper
		Interval(T iLower, T iUpper) : lower(iLower), upper(iUpper) { }

		//Getters and setters for members
		T getLower() const { return lower; }
		void setLower(T newLower) { lower = newLower; }
		T getUpper() const { return upper; }
		void setUpper(T newUpper) { upper = newUpper; }

		//Gets the distance between upper and lower
		T distance();

		//Checks if a value is inside the boundary
		bool insideInterval(T value);

		//Widens the boundary to be a superset of the boundary passed in
		void widen(const Interval<T>& other);
	private:
		T lower, upper;
};

template<typename T>
inline T Interval<T>::distance()
{
	return upper - lower;
}

template<typename T>
inline bool Interval<T>::insideInterval(T value)
{
	return (lower < value) && (value < upper);
}

template<typename T>
inline void Interval<T>::widen(const Interval<T>& other)
{
	if (other.lower < lower) setLower(other.lower);
	if (upper < other.upper) setUpper(other.upper);
}
