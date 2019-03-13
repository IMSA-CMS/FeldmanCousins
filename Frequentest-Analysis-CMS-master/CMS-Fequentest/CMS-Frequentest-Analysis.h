#pragma once
#pragma once

#include <vector>
//Factorial Function
double long factorial(int number);

//Given the parameters, a bin number, and a limit, calculates mu.
double newmufunction(std::vector<double> params, int munumber, float Beta);

//Generates a Pseudoexperiment given a mean (mu) value
int PE_Generator(double mean);

//Generates the Observed Values from the Parameters
std::vector<float> ObservedGenerator(std::vector<double> params);

//
std::vector<float> BackgroundGenerator(std::vector<double> params);

//New Ln(Likelihood Function) Approximation by Stirling
double NEW_LN_Likelihood_Function(float Limit, std::vector<double> params, std::vector<float> PEvector);

//Finds the Likelihood of just the background.
double NEW_LN_Likelihood_Function_Background(std::vector<double> params, std::vector<float> PEvector);

//Sorts vectors of floats by their last elements.
bool VectorSortingAlg(std::vector<float> i, std::vector<float> j);

//
int NEW_ninetyfivepercentgenerator(float LimitGuess, std::vector<double> params);