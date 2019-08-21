#include "stdafx.h"
#include <iostream>
#include <random>
#include <chrono>
#include <cmath>
#include <string>
#include <sstream>
#include <array>
#include <fstream>
#include <vector>
#include <limits>
#include <algorithm>

std::vector<std::vector<double>> RowSwap(std::vector<std::vector<double>> matrix, int row1, int row2) {
	std::vector<std::vector<double>> placeholder;
	for (int i = 0; i < matrix.size(); i++) {
		placeholder[i] = matrix[i][row1];
		matrix[i][row1] = matrix[i][row2];
		matrix[i][row2] = placeholder[i];
	}
	return matrix;
}

//multiples of row1 added to row2 until row2[zeroindex] = 0

std::vector<std::vector<double>> RowAdd(std::vector<std::vector<double>> matrix, int row1, int row2, int zeroindex) {
	double factor = matrix[zeroindex][row2] / matrix[zeroindex][row1];
	for (int i = 0; i < matrix.size(); i++) {
		matrix[i][row2] = matrix[i][row2] - factor * matrix[i][row1];
	}
	return matrix;
}

std::vector<double> gaussian_Elimination(std::vector<std::vector<double>> matrix) // in need of implementation
{
	std::vector<double> placeholder;
	std::vector<double> out;
	for (int i = 0; i < matrix.size() - 1; i++) {
		if (matrix[i][i] == 0) {
			for (int j = i; j < matrix[i].size(); j++) {
				if (matrix[i][j] != 0) {
					matrix = RowSwap(matrix, i, j);
					break;
				}
			}
		}
		for (int j = i + 1; j < matrix[i].size(); j++) {
			RowAdd(matrix, i, j, i);
		}
	}
	for (int i = matrix.size() - 2; i > 0; i--) {
		for (int j = 0; j < i; j++) {
			RowAdd(matrix, i, j, i);
		}
	}
	for (int i = 0; i < matrix[matrix.size() - 1].size() - 1; i++) {
		if (matrix[i][i] == 0) {
			out[i] = 1;
		}
		else {
			out[i] = matrix.end()[i] / matrix[i][i];
		}
	}
	return out;
}

int main() {
	std::vector<std::vector<double>> matrix = { {1,0,0},{0,1,0},{0,0,0} };
	std::vector test = gaussian_Elimination(matrix);
	for (int i = 0; i < test.size(); i++) {
		std::cout << test[i] << ", ";
	}
	return 0;
}