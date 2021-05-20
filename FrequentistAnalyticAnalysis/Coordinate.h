#ifndef FREQUENTIST_ANALYTIC_ANALYSIS_COORDINATE_H
#define FREQUENTIST_ANALYTIC_ANALYSIS_COORDINATE_H
#include <stdexcept>
#include <vector>
#include <initializer_list>
//Coordinate.h: Efficiently stores a coordinate in phase space with the number of dimensions determined at runtime.
// Provides access with bounds checking.
template<typename T>
class Coordinate
{
	public:
		//Builds the coordinate using its size
		Coordinate<T>(int size);

		//Access a specific dimension of Coordinate with bounds checking.
		T& operator[](int index);
		const T& operator[](int index) const;

		//Get number of dimensions
		int getDimensions() const { return static_cast<int>(coordinates.size()); }

		//Functions that allow for nice iteration (begin() and end() needed for foreach loop)
		T* begin() { return coordinates.data(); }
		T* end() { return coordinates.data() + getDimensions(); }
		const T* begin() const { return coordinates.data(); }
		const T* end() const { return coordinates.data() + getDimensions(); }

		//Testing for equality by comparing each dimension.
		bool operator==(const Coordinate<T>& other) const;
		bool operator!=(const Coordinate<T>& other) const;

		//Taking the sum of all of the dimensions.
		T sum() const;

		//Returning the product of all of the dimensions.
		T product() const;

		//Returns less than to order in row-major ordering
		bool lessThanRowMajor(const Coordinate<T>& other) const;
	private:
		std::vector<T> coordinates;
};

template<typename T>
inline T& Coordinate<T>::operator[](int index)
{
	if (index >= getDimensions())
		throw std::out_of_range("Attempted coordinate access out of bounds.");
	return coordinates[index];
}

template<typename T>
inline const T& Coordinate<T>::operator[](int index) const
{
	if (index >= getDimensions())
		throw std::out_of_range("Attempted coordinate access out of bounds.");
	return coordinates[index];
}

template<typename T>
inline bool Coordinate<T>::operator==(const Coordinate<T>& other) const
{
	if (getDimensions() != other.getDimensions())
		throw std::runtime_error("Cannot compare Coordinates with different getDimensions.");
	for (int i = 0; i < getDimensions(); ++i)
	{
		if (coordinates[i] != other.coordinates[i]) return false;
	}
	return true;
}

template<typename T>
inline bool Coordinate<T>::operator!=(const Coordinate<T>& other) const
{
	return !(*this == other);
}

template<typename T>
inline T Coordinate<T>::sum() const
{
	return coordinates.sum();
}

template<typename T>
inline T Coordinate<T>::product() const
{
	T product = 1;
	for (int i = 0; i < getDimensions(); ++i)
		product *= coordinates[i];
	return product;
}

template<typename T>
inline bool Coordinate<T>::lessThanRowMajor(const Coordinate<T>& other) const
{
	if (getDimensions() != other.getDimensions())
		throw std::runtime_error("Cannot do lessThanRowMajor comparison on\
		 Coordinates with different getDimensions.");
	if (*this == other)
		return false;
	for (int i = 0; i < getDimensions(); ++i)
	{
		if ((*this)[i] > other[i]) return false;
		if ((*this)[i] < other[i]) return true;
	}
	return false;
}

template<typename T>
inline Coordinate<T>::Coordinate(int size)
	: coordinates(size)
{
	
}
#endif