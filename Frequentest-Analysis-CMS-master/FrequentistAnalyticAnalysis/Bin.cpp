#include "Bin.h"
#include <stdexcept>
#include <vector>
Bin::Bin(std::vector<NMuPair> nMuPairs) :
	nArray(nMuPairs.size()),
	likelihoodArray(nMuPairs.size()),
	dimensions(nMuPairs.size())
{
	for (int i = 0; i < dimensions; ++i)
	{
		nArray[i] = nMuPairs[i].n;
		likelihoodArray[i] = calculateLnPoissonLikelihood(nMuPairs[i].n, nMuPairs[i].mu);
	}
}

int Bin::getN(int channelIndex) const
{
	return nArray[channelIndex];
}

double Bin::getLnLikelihood(int channelIndex) const
{
	return likelihoodArray[channelIndex];
}

double Bin::getTotalLnLikelihood() const
{
	return likelihoodArray.sum();
}

double Bin::calculateLnPoissonLikelihood(int n, double mu)
{
	return n * std::log(mu) - mu - std::lgamma(n + 1);
}

bool Bin::operator<(const Bin& other) const
{
	if (dimensions != other.dimensions)
		throw std::runtime_error("Bins are not comparable as they do not have the same number of dimensions.");
	double cmpOne = 0, cmpTwo = 0;
	for (int i = 0; i < dimensions; ++i)
	{
		cmpOne += getLnLikelihood(i);
		cmpTwo += other.getLnLikelihood(i);
		//More complex implementation for systematics
		//cmpOne += (getLnLikelihood(i) - calculateLnPoissonLikelihood(getN(i), getN(i))); 
		//cmpTwo += (other.getLnLikelihood(i) - calculateLnPoissonLikelihood(other.getN(i), other.getN(i)));
	}
	return cmpOne < cmpTwo;
}

void Bin::normalize(const Coordinate<double>& normalizationFactor)
{
	for (int i = 0; i < dimensions; ++i)
		likelihoodArray[i] -= normalizationFactor[i];
}

const Coordinate<int>& Bin::getAllN() const
{
	return nArray;
}
