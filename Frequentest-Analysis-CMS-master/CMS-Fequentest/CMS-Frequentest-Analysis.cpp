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
#include "CMS-Frequentest-Analysis.h"
#include <algorithm>
#include "Matrix.cpp"
#include "Orthogonal_Plane.h"


double long factorial(int number) {
	double long answer = 1;
	for (int i = 1; i<number + 1; i++) {
		answer = answer*i;
	}
	return answer;
}

double magnitude(std::vector<double> vect)
{
	double square_sum = 0;
	for (int i = 0; i < vect.size(); ++i)
	{
		square_sum += vect[i] * vect[i];
	}
	return sqrt(square_sum);
}

double newmufunction(std::vector<double> params, int munumber, double Beta) {
	double newmu = params[9 * munumber + 2] + params[9 * munumber + 3] * Beta + params[9 * munumber + 4] * Beta*Beta + params[9 * munumber + 5] + params[9 * munumber + 6];
	return newmu;
}

long long int PE_Generator(double mean)
{
	std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
	std::poisson_distribution<long> distribution(mean);
	long long int number;
	number = distribution(generator);
	return number;
}


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


bool VectorSortingAlg(std::vector<double> i, std::vector<double> j) {
	return(i.back() < j.back());
}

bool FirstVectorSortingAlg(std::vector<double> i, std::vector<double> j) {
	return(i.front() < j.front());
}

void shift_points(std::vector<std::vector<double>>& data, std::vector<double> shift)
{
	for (unsigned int i = 0; i < data.size(); ++i)
	{
		for (unsigned int j = 0; j < shift.size(); ++j)
		{
			data[i][j] -= shift[j];
		}
	}
}

double dot(std::vector<double> v1, std::vector<double> v2)
{
	double sum = 0;
	for (unsigned i = 0; i < v1.size(); ++i)
	{
		sum += v1[i] * v2[i];
	}
	return sum;
}

std::vector<double> scalar_product(double s, std::vector<double> v)
{
	std::vector<double> returnVector;
	for (int i = 0; i < v.size(); ++i)
	{
		returnVector.push_back(v[i] * s);
	}
	return returnVector;
}

std::vector<double> elementwise_product(std::vector<double> v1, std::vector<double> v2)
{
	std::vector<double> returnVector;
	for (int i = 0; i < v1.size(); ++i)
	{
		returnVector.push_back(v1[i] * v2[i]);
	}
	return returnVector;
}

std::vector<double> elementwise_division(std::vector<double> v1, std::vector<double> v2)
{
	std::vector<double> returnVector;
	for (int i = 0; i < v1.size(); ++i)
	{
		returnVector.push_back(v1[i] / v2[i]);
	}
	return returnVector;
}

std::vector<double> elementwise_sqrt(std::vector<double> v)
{
	std::vector<double> returnVector;
	for (int i = 0; i < v.size(); ++i)
	{
		returnVector.push_back(sqrt(v[i]));
	}
	return returnVector;
}

std::vector<double> elementwise_constant_sum(double s, std::vector<double> v)
{
	std::vector<double> returnVector;
	for (int i = 0; i < v.size(); ++i)
	{
		returnVector.push_back(v[i] + s);
	}
	return returnVector;
}

std::vector<double> vector_sum(std::vector<double> v1, std::vector<double> v2)
{
	std::vector<double> returnVector;
	for (int i = 0; i < v1.size(); ++i)
	{
		returnVector.push_back(v1[i] + v2[i]);
	}
	return returnVector;
}

std::vector<double> flip_vect(std::vector<double> v1)
{
	for (int i = 0; i < v1.size(); ++i)
	{
		v1[i] *= -1;
	}
	return v1;
}

std::vector<double> unit_vect(std::vector<double> v)
{
	double sum = 0;
	for (int i = 0; i < v.size(); ++i)
		sum += v[i]*v[i];
	sum = sqrt(sum);
	std::vector<double> outvec;
	for (int i = 0; i < v.size(); ++i)
		outvec.push_back(v[i] / sum);
	return outvec;
}

long int in_front(std::vector<double> vect, std::vector<std::vector<double>>& data)
{
	long int front = 0;
	const double small = 0.00000001;
	for (unsigned i = 0; i < data.size(); ++i)
	{
		if (dot(vect, data[i]) > small)
			++front;
	}
	return front;
}

bool check_vect(std::vector<double> vect, std::vector<std::vector<double>>& data)
{
	double front = in_front(vect, data);
	if (front == 0 || front == data.size())
	{
		return true;
	}
	return false;
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


bool NEW_ninetyfivepercentgenerator(double LimitGuess, std::vector<double> params, int GeneratedNumber) {
	int PEnumber = GeneratedNumber;
	double fivep = 0.05 * PEnumber;
	int binnumber = params.size() / 9;
	int closepointnumber = binnumber * 10;
	double pointdistance = 0;
	std::vector<double> finalrange;
	std::vector<std::vector<double> > CoupledNvector(PEnumber);
	//std::vector<std::vector<double> > CloseNvector(closepointnumber);
	std::vector<std::vector<double> > Nvector(PEnumber);
	std::vector<double> DistanceVector;
	for (int i = 0; i < PEnumber; i++) {
		Nvector[i].resize(binnumber);
		CoupledNvector[i].resize(binnumber + 1);
		//CloseNvector.resize(binnumber + 1);
		for (int j = 0; j < binnumber; j++) {
			Nvector[i][j] = PE_Generator(newmufunction(params, j, LimitGuess));
			CoupledNvector[i][j] = Nvector[i][j];
		}
		CoupledNvector[i][binnumber] = NEW_LN_Likelihood_Function(LimitGuess, params, Nvector[i])
			- NEW_LN_Likelihood_Function_Background(params, Nvector[i]);
		//std::cout << Nvector[i][0] << std::endl;
		//std::cout << NEW_LN_Likelihood_Function(LimitGuess, params, Nvector[i]) << std::endl;
		//std::cout << NEW_LN_Likelihood_Function_Background(params, Nvector[i]) << std::endl;
	}
	std::sort(CoupledNvector.begin(), CoupledNvector.end(), VectorSortingAlg);
	// Sorts all of the likelihoods
	CoupledNvector.erase(CoupledNvector.begin(), CoupledNvector.begin() + (int)fivep);
	// Gets rid of 5% of the data
	std::vector<std::vector<double>> dataSet;
	std::vector<double> temp;
	for (int i = 0; i < CoupledNvector.size(); ++i)
	{
		for (int j = 0; j < CoupledNvector[0].size(); ++j)
		{
			temp.push_back(CoupledNvector[i][j]);
		}
		dataSet.push_back(temp);
		temp.clear();
	}

	for (int i = 0; i < dataSet.size(); ++i)
	{
		dataSet[i].erase(dataSet[i].end() - 1);
	}
	std::vector<double> test = ObservedGenerator(params);
	shift_points(dataSet, test); //Shifting the points so that the origin is the observed point
	std::vector<std::vector<double>> pointSet;
	for (int i = 0; i < dataSet.size(); ++i)
	{
		pointSet.push_back(unit_vect(dataSet[i]));
	}
	Orthogonal_Plane cPlane(pointSet);
	return cPlane.Check_Plane();

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





/*std::vector<double> gaussian_Elimination(std::vector<std::vector<double>> inmatrix)
{
	std::vector<std::vector<double>> matrix = inmatrix;
	unsigned int row = matrix.size();
	unsigned int column = matrix[0].size();
	
	to_reduced_row_echelon_form(matrix);

	


	std::vector<double> null_space(matrix[0].size(),0);
	auto last_row = matrix[matrix.size() - 1];

	//build the null space to work with the last row
	if (last_row[last_row.size() - 1] == 0)
		null_space[null_space.size()-1] = 1;

	else if (last_row[last_row.size() - 2] == 0)
		null_space[null_space.size()-2] = 1;
	else
	{
		null_space[null_space.size() - 2] = -last_row[last_row.size() - 1];
		null_space[null_space.size() - 1] = last_row[last_row.size() - 2];
	}
		
	//build the rest of the null space
	for (int i = matrix.size() - 2; i >= 0; --i)
	{
		if (matrix[i][i] == 0)
		{
			std::vector<double> null_space(matrix[0].size(), 0);
			null_space[i] = 1;
		}
		else
		{
			double sum = 0;
			for (unsigned j = 0; j < null_space.size(); ++j)
			{
				sum += null_space[j] * matrix[i][j];
			}
			double new_term = -1 * sum / matrix[i][i];
			null_space[i] = new_term;
		}
		
	}
	return null_space;
}
*/




int main()
{
	std::string line;
	std::vector<double> parametervector;
	std::fstream parameterfile;
	parameterfile.open("correctedparameterfile.txt");
	while (std::getline(parameterfile, line)) 
		parametervector.push_back(std::stod(line));
	
	//double nfpercentaim = 0;
	//for(int i=0; i<parametervector.size()/9; i++){
	//  nfpercentaim = nfpercentaim + ObservedGenerator(parametervector)[i]*ObservedGenerator(parametervector)[i];
	//}
	//nfpercentaim = std::sqrtluw(nfpercentaim);
	/*
	double lowerLimit = 7;
	double upperLimit = 20;
	int numberOfSegments = 13;
	int genNum = 10000000;
	for (int i = 0; i<numberOfSegments; ++i) {
		double lambda = lowerLimit + (upperLimit - lowerLimit) * i / numberOfSegments;
		double beta = 1 / (lambda * lambda);
		std::cout << NEW_ninetyfivepercentgenerator(beta, parametervector, genNum)
			<< std::endl << lambda << std::endl << std::endl;
	}
	*/
	const double lambdaTwo = 40;
	int genNum = 100000000;
	std::cout << NEW_ninetyfivepercentgenerator(1 / (lambdaTwo * lambdaTwo), parametervector, genNum)
		<< std::endl << lambdaTwo << std::endl << std::endl;
	//Given an upper expected L and a number of segments to break it into, prints the segments that return the correct answer (95% FOM Limit = FOM for a pseudoexperiment returning Observed Values)
	//std::cout << newmufunction(parametervector, 0, 0.01) << std::endl;
	//std::cout << "Lambda = 10 Expected Value: " << NEW_ninetyfivepercentgenerator(0.01, parametervector)[0] << "          " << NEW_ninetyfivepercentgenerator(0.01, parametervector)[1] << std::endl;
	//std::cout << "Lambda = 7.31 Expected Value: " << NEW_ninetyfivepercentgenerator(0.0187139406, parametervector)[0] <<  "          " << NEW_ninetyfivepercentgenerator(0.0187139406, parametervector)[1] << std::endl;
	//std::cout << "Lambda = 7.76 Expected Value: " << NEW_ninetyfivepercentgenerator(0.0166064406, parametervector)[0] <<  "          " << NEW_ninetyfivepercentgenerator(0.0166064406, parametervector)[1] << std::endl;
	
	return 0;
}



//EE BB Lambda 10 DesLL
//		Frequentist		Half-Space	Bayesian	
//		 15.47			25.5		21.14		
//	
//BIN 1: 7.01			7.01		4.02
//BIN 2: 11.97			11.97		6.40
//BIN 3: 11.26			11.26		9.50
//BIN 4: 15.90			15.90		15.78
//BIN 5: 19.94			19.94		20.71
//BIN 6: 14.57			14.57		16.21