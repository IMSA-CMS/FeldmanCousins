#pragma once

#include <vector>
//Factorial Function
double long factorial(int number);

//Given the parameters, a bin number, and a limit, calculates mu.
double newmufunction(std::vector<double> params, int munumber, double Beta);

//Generates a Pseudoexperiment given a mean (mu) value
int PE_Generator(double mean);

//Generates the Observed Values from the Parameters
std::vector<double> ObservedGenerator(std::vector<double> params);

//
std::vector<double> BackgroundGenerator(std::vector<double> params);

//New Ln(Likelihood Function) Approximation by Stirling
double NEW_LN_Likelihood_Function(double Limit, std::vector<double> params, std::vector<double> PEvector);

//Finds the Likelihood of just the background.
double NEW_LN_Likelihood_Function_Background(std::vector<double> params, std::vector<double> PEvector);

//Sorts vectors of doubles by their last elements.
bool VectorSortingAlg(std::vector<double> i, std::vector<double> j);

//
bool NEW_ninetyfivepercentgenerator(double LimitGuess, std::vector<double> params);

//Finds the vector with the smallest index from a vector of vectors
std::vector<double> find_Smallest_By_Index(std::vector<std::vector<double>> data, int index);

//Finds the vector with the smallest index from a vector of vectors
std::vector<double> find_Largest_By_Index(std::vector<std::vector<double>> data, int index);

//Takes an input "matrix" and outputs a solution vector via guassian elimination
std::vector<double> gaussian_Elimination(std::vector<std::vector<double>> matrix);

//Preforms a bubble sort on a vector of vectors for some index
bool sortcol(const std::vector<int>& v1, const std::vector<int>& v2);
void bubSort(std::vector<std::vector<double>>& data, int index);