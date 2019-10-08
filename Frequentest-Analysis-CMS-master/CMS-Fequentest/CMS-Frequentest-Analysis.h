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

//Sorts vectors of doubles by their first elements.
bool FirstVectorSortingAlg(std::vector<double> i, std::vector<double> j);

//
bool NEW_ninetyfivepercentgenerator(double LimitGuess, std::vector<double> params);

//Takes an input "matrix" and outputs a solution vector for the nullspace via guassian elimination
std::vector<double> gaussian_Elimination(std::vector<std::vector<double>> matrix);


//multiples of row1 added to row2 until row2[zeroindex] = 0
std::vector<std::vector<double>> RowAdd(std::vector<std::vector<double>> inmatrix, int row1, int row2, int zeroindex);

//Preforms a row swap between row1 and row2 of inmatrix
std::vector<std::vector<double>> RowSwap(std::vector<std::vector<double>> inmatrix, int row1, int row2);

//Takes the transpose of matrix
std::vector<std::vector<double>> Transpose(std::vector<std::vector<double>> matrix);