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
		likelihoodArray[i] = PoissonLikelihood(nMuPairs[i].n, nMuPairs[i].mu);
	}
}

int Bin::getN(int channelIndex) const
{
	return nArray[channelIndex];
}

const PoissonLikelihood& Bin::getLnLikelihood(int channelIndex) const
{
	return likelihoodArray[channelIndex];
}

double Bin::getTotalLnLikelihood() const
{
	double accumulate = 0;
	for (int i = 0; i < likelihoodArray.getDimensions(); ++i)
	{
		accumulate += likelihoodArray[i].toLnLikelihood();
	}
	return accumulate;
}

bool Bin::operator<(const Bin& other) const
{
	if (dimensions != other.dimensions)
		throw std::runtime_error("Bins are not comparable as they do not have the same number of dimensions.");
	double cmpOne = 0, cmpTwo = 0;

	for (int i = 0; i < dimensions; ++i)
	{
		cmpOne += getLnLikelihood(i).lnRatio(PoissonLikelihood(getN(i), getN(i)));
		cmpTwo += other.getLnLikelihood(i).lnRatio(PoissonLikelihood(getN(i), getN(i)));
	}
	return cmpOne < cmpTwo;
}

//void Bin::normalize(const Coordinate<double>& normalizationFactor)
//{
	//Code has gone too far to save this method
	//Please do not use
	//for (int i = 0; i < dimensions; ++i)
		//likelihoodArray[i] = .toLnLikelihood() -= normalizationFactor[i];
//}

const Coordinate<int>& Bin::getAllN() const
{
	return nArray;
}
