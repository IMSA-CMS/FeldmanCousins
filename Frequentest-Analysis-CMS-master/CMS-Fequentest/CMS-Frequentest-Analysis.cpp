// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

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
#include "Hyper_Surface.h"
#include "CMS-Frequentest-Analysis.h"
#include <algorithm>

//Factorial Function

double long factorial(int number) {
	double long answer = 1;
	for (int i = 1; i<number + 1; i++) {
		answer = answer*i;
	}
	return answer;
}

//Given the parameters, a bin number, and a limit, calculates mu.

double newmufunction(std::vector<double> params, int munumber, double Beta) {
	double newmu = params[9 * munumber + 2] + params[9 * munumber + 3] * Beta + params[9 * munumber + 4] * Beta*Beta + params[9 * munumber + 5] + params[9 * munumber + 6];
	return newmu;
}

//Generates a Pseudoexperiment given a mean (mu) value

int PE_Generator(double mean)
{
	std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
	std::poisson_distribution<int> distribution(mean);
	int number = distribution(generator);
	return number;
}

//Generates the Observed Values from the Parameters

std::vector<double> ObservedGenerator(std::vector<double> params) {
	std::vector<double> observed;
	for (int i = 0; i<params.size() / 9; i++) {
		observed.push_back(params[9 * i + 7]);
	}
	return observed;
}

std::vector<double> BackgroundGenerator(std::vector<double> params) {
	std::vector<double> background;
	for (int i = 0; i<params.size() / 9; i++) {
		background.push_back(params[9 * i + 5] + params[9 * i + 6]);
	}
	return background;
}

//New Ln(Likelihood Function) Approximation by Stirling

double NEW_LN_Likelihood_Function(double Limit, std::vector<double> params, std::vector<double> PEvector) {
	std::vector<double> muvector;
	double likelihood = 0;
	for (int i = 0; i<params.size() / 9; i++) {
		double mu = newmufunction(params, i, Limit);
		muvector.push_back(mu);
		if (PEvector[i] != 0) {
			likelihood = likelihood + PEvector[i] * std::log(muvector[i]) - muvector[i] - PEvector[i] * std::log(PEvector[i]) + PEvector[i];
		}
		if (PEvector[i] == 0) {
			likelihood = likelihood - std::log(factorial(PEvector[i])) + PEvector[i] * std::log(muvector[i]) - muvector[i];
		}
	}
	return likelihood;
}

//Finds the Likelihood of just the background.

double NEW_LN_Likelihood_Function_Background(std::vector<double> params, std::vector<double> PEvector) {
	double likelihood = 0;
	std::vector<double> muvector = BackgroundGenerator(params);
	for (int i = 0; i<params.size() / 9; i++) {
		if (PEvector[i] != 0) {
			likelihood = likelihood + PEvector[i] * std::log(muvector[i]) - muvector[i] - PEvector[i] * std::log(PEvector[i]) + PEvector[i];
		}
		if (PEvector[i] == 0) {
			likelihood = likelihood - std::log(factorial(PEvector[i])) + PEvector[i] * std::log(muvector[i]) - muvector[i];
		}
		if (muvector[i] == 0) {
			std::cout << "ERROR: Background of 0 not allowed" << std::endl;
		}
	}
	return likelihood;
}

//Sorts vectors of doubles by their last elements.

bool VectorSortingAlg(std::vector<double> i, std::vector<double> j) {
	return(i.back()<j.back());
}

//std::vector<double> Center_Of_Mass(std::vector<std::vector<double>> Region, std::vector<double> Observed) {
//	std::vector<double> COM;
//	for (int i = 0; i<Observed.size(); i++) {
//		double total = 0;
//		for (int j = 0; j<Region.size(); j++) {
//			total = total + Region[j][i];
//		}
//		total = total / Region.size();
//		COM.push_back(total);
//	}
//	return COM;
//}
//
//double DistanceFunction(std::vector<double> Point, std::vector<double> Observed) {
//	double distance = 0;
//	for (int i = 0; i<Observed.size(); i++) {
//		distance = distance + (Point[i] - Observed[i])*(Point[i] - Observed[i]);
//	}
//	return distance;
//}
//
//std::vector<double> NormalVector(std::vector<double> COM, std::vector<double> NearObserved) {
//	std::vector<double> Normal;
//	for (int i = 0; i<COM.size(); i++) {
//		Normal.push_back(COM[i] - NearObserved[i]);
//	}
//	return Normal;
//}
//
//double constant_term(std::vector<double> Normal, std::vector<double> NearObserved) {
//	double constantterm = 0;
//	for (int i = 0; i<Normal.size(); i++) {
//		constantterm = constantterm + Normal[i] * NearObserved[i];
//	}
//	return constantterm;
//}


// //Given an L, returns the 95% bound for the FOMs using the other functions

bool NEW_ninetyfivepercentgenerator(double LimitGuess, std::vector<double> params) {
	int PEnumber = 10;
	double fivep = 0.05*PEnumber;
	int binnumber = params.size() / 9;
	int closepointnumber = binnumber * 10;
	double pointdistance = 0;
	std::vector<double> finalrange;
	std::vector<std::vector<double> > CoupledNvector(PEnumber);
	//std::vector<std::vector<double> > CloseNvector(closepointnumber);
	std::vector<std::vector<double> > Nvector(PEnumber);
	std::vector<double> DistanceVector;
	int bad = 1;
	for (int i = 0; i<PEnumber; i++) {
		Nvector[i].resize(binnumber);
		CoupledNvector[i].resize(binnumber + 1);
		//CloseNvector.resize(binnumber + 1);
		for (int j = 0; j<binnumber; j++) {
			Nvector[i][j] = PE_Generator(newmufunction(params, j, LimitGuess));
			CoupledNvector[i][j] = Nvector[i][j];
		}
		CoupledNvector[i][binnumber] = NEW_LN_Likelihood_Function(LimitGuess, params, Nvector[i]) - NEW_LN_Likelihood_Function_Background(params, Nvector[i]);
		//std::cout << Nvector[i][0] << std::endl;
		//std::cout << NEW_LN_Likelihood_Function(LimitGuess, params, Nvector[i]) << std::endl;
		//std::cout << NEW_LN_Likelihood_Function_Background(params, Nvector[i]) << std::endl;
	}
	std::sort(CoupledNvector.begin(), CoupledNvector.end(), VectorSortingAlg);
	CoupledNvector.erase(CoupledNvector.begin(), CoupledNvector.begin() + (int)fivep);
	std::vector<std::vector<double>> dataSet;
	std::vector<double> temp;
	for (std::size_t i = 0; i < CoupledNvector.size(); ++i)
	{
		for (std::size_t j = 0; j < CoupledNvector[0].size()-1; ++j)
		{
			temp.push_back(CoupledNvector[i][j]);
		}
		dataSet.push_back(temp);
		temp.clear();
	}
	std::cout << temp[0] << std::endl;
	Hyper_Surface surface(dataSet);
	surface.make_Surface();
	return surface.point_Is_In(ObservedGenerator(params));
	/*	for (int i = 0; i < CoupledNvector.size(); i++) {
			double distance = 0;
			for (int j = 0; j < CoupledNvector[i].size()-1; j++) {
				distance = (ObservedGenerator(params)[j] - CoupledNvector[i][j])*(ObservedGenerator(params)[j] - CoupledNvector[i][j]);
			}
			CoupledNvector[i][binnumber] = distance;
		}
		std::sort(CoupledNvector.begin(), CoupledNvector.end(), VectorSortingAlg);
		for (int i = 0; i < closepointnumber; i++) {
			double distance = 0;
			for (int j = 0; j < binnumber; j++) {
				CloseNvector[i][j] = CoupledNvector[CoupledNvector.size() - i - 1][j];
				distance = distance + CloseNvector[i][j] * CloseNvector[i][j];
			}
			CloseNvector[i][binnumber] = distance;
		}		
		for (int i = 0; i < binnumber; i++) {
			pointdistance = pointdistance + ObservedGenerator(params)[i] * ObservedGenerator(params)[i];
		}
		*/
//		std::sort(CloseNvector.begin(), CloseNvector.end(), VectorSortingAlg);
	//	if (CloseNvector[closepointnumber-1][binnumber] > pointdistance) {
		//	bad = 1;
	//	}

			/*	for (int j = 0; j < binnumber; j++) {
					for (int i = 0; i < CoupledNvector.size(); i++) {
						CoupledNvector[i][binnumber] = CoupledNvector[i][j];
					}
					std::sort(CoupledNvector.begin(), CoupledNvector.end(), VectorSortingAlg);
					if (CoupledNvector[0][j] > ObservedGenerator(params)[j] || CoupledNvector.back()[j] < ObservedGenerator(params)[j]) {
						bad = 0;
						//std::cout << ObservedGenerator(params)[j] << std::endl;
						//std::cout << CoupledNvector[0].back() << std::endl;
						//std::cout << CoupledNvector.back().back() << std::endl;
					}
					std::cout << CoupledNvector[0][j] << "            " << CoupledNvector.back()[j] << "           " << ObservedGenerator(params)[j] << std::endl;
				}*/
	//for (int i = 0; i<PEnumber - (int)fivep; i++) {
	//	CoupledNvector[i][binnumber] = DistanceFunction(Nvector[i], ObservedGenerator(params));
	//}
	//std::sort(CoupledNvector.begin(), CoupledNvector.end(), VectorSortingAlg);
	//double firstcomparison = 0;
	//for (int i = 0; i<ObservedGenerator(params).size(); i++) {
	//	firstcomparison = firstcomparison + NormalVector(Center_Of_Mass(CoupledNvector, ObservedGenerator(params)), CoupledNvector[0])[i] * CoupledNvector[1][i];
	//}
	//std::cout << "firstcomparison: " << firstcomparison << std::endl;
	//bool edge = true;
	//for (int i = 0; i<PEnumber - (int)fivep; i++) {
	//	double comparison = 0;
	//	for (int j = 0; j<ObservedGenerator(params).size(); j++) {
	//		comparison = comparison + NormalVector(Center_Of_Mass(CoupledNvector, ObservedGenerator(params)), CoupledNvector[0])[j] * CoupledNvector[i][j];
			//std::cout << NormalVector(Center_Of_Mass(CoupledNvector), CoupledNvector[0])[j];
			//std::cout << Center_Of_Mass(CoupledNvector)[0];
	//	}
		//std::cout << "comparison: " << comparison << std::endl;
	//	if (comparison / firstcomparison < 0) {
	//		edge = false;
	//	}
	//}
	//   for(int i=0; i<PEnumber-(int)fivep; i++){
	//   double distancesquared=0;
	//   for(int j=0; j<binnumber; j++){
	//     distancesquared = distancesquared+CoupledNvector[i][j]*CoupledNvector[i][j];      
	//   }
	//   DistanceVector.push_back(std::sqrt(distancesquared));
	// }
	// std::sort(DistanceVector.begin(), DistanceVector.end());
	// finalrange.push_back(DistanceVector[0]);
	// finalrange.push_back(DistanceVector.back());
	//return(finalrange);
	//return(bad);
}

std::vector<double> find_Smallest_By_Index(std::vector<std::vector<double>> data, int index)
{
	std::vector<double> out;
	for (std::size_t i = 0; i < data.size(); ++i)
	{
		if (i == 0)
		{
			out = data[i];
		}
		else
		{
			if (data[i][index] < out[index])
			{
				out = data[i];
			}
		}
	}
	return out;
}

std::vector<double> find_Largest_By_Index(std::vector<std::vector<double>> data, int index)
{
	std::vector<double> out;
	for (std::size_t i = 0; i < data.size(); ++i)
	{
		if (i == 0)
		{
			out = data[i];
		}
		else
		{
			if (data[i][index] > out[index])
			{
				out = data[i];
			}
		}
	}
	return out;
}

std::vector<std::vector<double>> RowSwap(std::vector<std::vector<double>> inmatrix, int row1, int row2) {
	std::vector<double> placeholder;
	std::vector<std::vector<double>> matrix = inmatrix;
	for (int i = 0; i < matrix.size(); i++) {
		placeholder.push_back(matrix[i][row1]);
		matrix[i][row1] = matrix[i][row2];
		matrix[i][row2] = placeholder[i];
	}
	return matrix;
}

std::vector<std::vector<double>> Transpose(std::vector<std::vector<double>> matrix) {
	std::vector<std::vector<double>> outmatrix;
	outmatrix.resize(matrix[0].size());
	for (int i = 0; i < outmatrix.size(); i++)
		outmatrix[i].resize(matrix.size());
	for (int i = 0; i < matrix.size(); i++) {
		for (int j = 0; j < matrix[i].size(); j++) {
			outmatrix[j][i] = matrix[i][j];
		}
	}
	return outmatrix;
}


std::vector<std::vector<double>> RowAdd(std::vector<std::vector<double>> inmatrix, int row1, int row2, int zeroindex) {
	std::vector<std::vector<double>> matrix = inmatrix;
	double factor = matrix[zeroindex][row2] / matrix[zeroindex][row1];
	for (int i = 0; i < matrix.size(); i++) {
		matrix[i][row2] = matrix[i][row2] - (factor * matrix[i][row1]);
	}
	return matrix;
}

 

std::vector<double> gaussian_Elimination(std::vector<std::vector<double>> inmatrix) // in need of implementation
{
	std::vector<std::vector<double>> matrix = inmatrix;
	matrix = Transpose(matrix);
	std::vector<double> out(matrix.size());
	bool nonZero = true;
	for (int i = 0; i < matrix[0].size(); ++i)
	{
		if (matrix[i][i] == 0)
		{
			nonZero = false;
			for (int j = i; j < matrix[0].size(); ++j)
			{
				if (matrix[i][j] != 0)
				{
					matrix = RowSwap(matrix, i, j);
					nonZero = true;
					break;
				}
			}
		}
		if (nonZero)
		{
			for (int j = i + 1; j < matrix[0].size(); ++j)
			{
				matrix = RowAdd(matrix, i, j, i);
			}
		}
	}

	std::vector<bool> pivots(matrix.size());
	for (int i = 0; i < matrix[0].size(); ++i)
	{
		for (int j = 0; j < matrix.size(); ++j)
		{
			if (matrix[j][i] != 0)
			{
				pivots[j] = true;
				break;
			}
		}
	}

	for (int i = 0; i < pivots.size(); ++i)
	{
		if (!pivots[i])
		{
			out[i] = 1;
			break;
		}
	}
	double constant;
	int firstNonZero;
	for (int i = matrix[0].size() - 1; i >= 0; --i)
	{
		constant = 0;
		for (int j = 0; j < matrix.size(); ++j)
		{
			constant += matrix[j][i] * out[j];
		}
		for (int j = 0; j < matrix.size(); ++j)
		{
			if (matrix[j][i] != 0)
			{
				firstNonZero = j;
				break;
			}
		}
		if (i != matrix[0].size() - 1 || firstNonZero != matrix.size() - 1)
		{
			out[firstNonZero] = -1 * (constant / matrix[firstNonZero][i]);
		}
	}
	return out;
}



bool sortcol(const std::vector<double>& v1, const std::vector<double>& v2) {
	return v1[0] < v2[0];
}

void bubSort(std::vector<std::vector<double>>& data, int index)
{
	std::vector<double> temp;
	for (std::size_t i = 0; i < sizeof(data[0])-1; ++i)
	{

		if (sortcol(data[i], data[i + 1]))
		{
			temp = data[i];
			data[i] = data[i + 1];
			data[i + 1] = temp;
		}
		if (i == sizeof(data[0]) - 1)
		{
			return;
		}
	}
	bubSort(data, index);
}

int main()
{

	std::vector<std::vector<double>> data = { {1,1,2},{3,3,4},{1,4,7} };
	Hyper_Plane x(data);

	std::vector<double> point = { 4,4,4 };

	std::cout << x.dist_To_Point(point) << std::endl;

	return 0;

	/*
	std::string line;
	std::vector<double> parametervector;
	std::fstream parameterfile;
	parameterfile.open("testtocompare.txt");
	while (std::getline(parameterfile, line)) {
		parametervector.push_back(std::stod(line));
	}
	//double nfpercentaim = 0;
	//for(int i=0; i<parametervector.size()/9; i++){
	//  nfpercentaim = nfpercentaim + ObservedGenerator(parametervector)[i]*ObservedGenerator(parametervector)[i];
	//}
	//nfpercentaim = std::sqrt(nfpercentaim);
	double lowerLimit = 117;
	double upperLimit = 120;
	double numberOfSegments = 30;
	//Given an upper expected L and a number of segments to break it into, prints the segments that return the correct answer (95% FOM Limit = FOM for a pseudoexperiment returning Observed Values)
	//std::cout << newmufunction(parametervector, 0, 0.01) << std::endl;
	//std::cout << "Lambda = 10 Expected Value: " << NEW_ninetyfivepercentgenerator(0.01, parametervector)[0] << "          " << NEW_ninetyfivepercentgenerator(0.01, parametervector)[1] << std::endl;
	//std::cout << "Lambda = 7.31 Expected Value: " << NEW_ninetyfivepercentgenerator(0.0187139406, parametervector)[0] <<  "          " << NEW_ninetyfivepercentgenerator(0.0187139406, parametervector)[1] << std::endl;
	//std::cout << "Lambda = 7.76 Expected Value: " << NEW_ninetyfivepercentgenerator(0.0166064406, parametervector)[0] <<  "          " << NEW_ninetyfivepercentgenerator(0.0166064406, parametervector)[1] << std::endl;
	for (int i = 0; i<numberOfSegments; i++) {
		std::cout << NEW_ninetyfivepercentgenerator(lowerLimit + (upperLimit - lowerLimit)*i / numberOfSegments, parametervector) << std::endl << lowerLimit + (upperLimit - lowerLimit)*i / numberOfSegments << std::endl << std::endl;
	}
	return 0;
	*/
}



//EE BB Lambda 10 DesLL
//		Frequentist		Bayesian
//		 15.47			21.14
//
//BIN 1: 7.01			4.02
//BIN 2: 11.97			6.40
//BIN 3: 11.26			9.50
//BIN 4: 15.90			15.78
//BIN 5: 19.94			20.71
//BIN 6: 14.57			16.21