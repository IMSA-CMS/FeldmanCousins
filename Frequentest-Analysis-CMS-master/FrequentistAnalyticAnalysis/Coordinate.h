#pragma once
#include <stdexcept>
#include <valarray>
//Coordinate.h: Efficiently stores a coordinate in phase space with the number of dimensions determined at runtime.
// Provides access with bounds checking.
template<typename T>
class Coordinate
{
	public:
		//Builds
		Coordinate<T>(int iDimensions) : coordinates(iDimensions) {};

		//Access a specific dimension of Coordinate with bounds checking.
		T& operator[](int index);
		const T& operator[](int index) const;

		//Get number of dimensions
		int getDimensions() const { return static_cast<int>(coordinates.size()); }

		//Functions that allow for nice iteration (begin() and end() needed for foreach loop)
		T* begin() { return &(coordinates[0]); }
		T* end() { return &(coordinates[getDimensions()]); }
		const T* begin() const { return &(coordinates[0]); }
		const T* end() const { return &(coordinates[getDimensions()]); }

		//Testing for equality by comparing each dimension.
		bool operator==(const Coordinate<T>& other) const;
		bool operator!=(const Coordinate<T>& other) const;

		//Taking the sum of all of the dimensions.
		T sum() const;

		//Returning the product of all of the dimensions.
		T product() const;

	private:
		std::valarray<T> coordinates;
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
