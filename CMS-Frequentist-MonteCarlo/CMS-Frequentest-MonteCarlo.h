#ifndef CMS_FREQUENTIST_MONTECARLO_MAIN_H
#define CMS_FREQUENTIST_MONTECARLO_MAIN_H
#include <vector>
//Factorial Function
double long factorial(int number);

//Magnitude of vect
double magnitude(std::vector<double> vect);

//Given the parameters, a bin number, and a limit, calculates mu.
double newmufunction(std::vector<double> params, int munumber, double Beta);

//Generates a Pseudoexperiment given a mean (mu) value
long long int PE_Generator(double mean);

//Generates the Observed Values from the Parameters
std::vector<double> ObservedGenerator(std::vector<double> params);

//
std::vector<double> BackgroundGenerator(std::vector<double> params);

//New Ln(PoissonLikelihood Function) Approximation by Stirling
double NEW_LN_Likelihood_Function(double Limit, std::vector<double> params, std::vector<double> PEvector);

//Finds the PoissonLikelihood of just the background.
double NEW_LN_Likelihood_Function_Background(std::vector<double> params, std::vector<double> PEvector);

//Sorts vectors of doubles by their last elements.
bool VectorSortingAlg(std::vector<double> i, std::vector<double> j);

//Sorts vectors of doubles by their first elements.
bool FirstVectorSortingAlg(std::vector<double> i, std::vector<double> j);

//Subtracts each point in data by shift
void shift_points(std::vector<std::vector<double>> &data, std::vector<double> shift);

//Takes the dot product between v1 and v2
double dot(std::vector<double> v1, std::vector<double> v2);

//Takes the scalar product of s and v
std::vector<double> scalar_product(double s, std::vector<double> v);

//Takes the elementwise product of v1 and v2
std::vector<double> elementwise_product(std::vector<double> v1, std::vector<double> v2);

std::vector<double> elementwise_division(std::vector<double> v1, std::vector<double> v2);

std::vector<double> elementwise_sqrt(std::vector<double> v);

std::vector<double> elementwise_constant_sum(double s, std::vector<double> v);

//Takes the vector sum of v1 and v2
std::vector<double> vector_sum(std::vector<double> v1, std::vector<double> v2);

std::vector<double> flip_vect(std::vector<double> v1);

std::vector<double> unit_vect(std::vector<double> v);

//Finds the number of points in the direction of the vector
long int in_front(std::vector<double> vect, std::vector<std::vector<double>>& data);

//Checks if vect describes a plane which has all points on one side of it
bool check_vect(std::vector<double> vect, std::vector<std::vector<double>>& data);

//Given an L, returns the 95% bound for the FOMs using the other functions
bool NEW_ninetyfivepercentgenerator(double LimitGuess, std::vector<double> params,int GeneratedNumber);

//Takes an input "matrix" and outputs a solution vector for the nullspace via guassian elimination
std::vector<double> gaussian_Elimination(std::vector<std::vector<double>> matrix);
#endif