#include "SingleChannelPoissonDistribution.h"
#include <algorithm>
#include <tuple>
#include <cmath>

SingleChannelPoissonDistribution::Bin::Bin(int iN, double iMu)
	: n(iN),
	lnLikelihoodMu(calculateLnPoissonLikelihood(iN, iMu)),
	lnLikelihoodN(calculateLnPoissonLikelihood(iN, iN))
{ }

bool SingleChannelPoissonDistribution::Bin::lessThanCmpFeldmanCousins
	(const SingleChannelPoissonDistribution::Bin& one, const SingleChannelPoissonDistribution::Bin& two)
{
	return (one.lnLikelihoodMu - one.lnLikelihoodN) > (two.lnLikelihoodMu - two.lnLikelihoodN);
}

bool SingleChannelPoissonDistribution::Bin::lessThanCmpN
	(const SingleChannelPoissonDistribution::Bin& one, const SingleChannelPoissonDistribution::Bin& two)
{
	return one.n < two.n;
}

double SingleChannelPoissonDistribution::Bin::calculateLnPoissonLikelihood(int n, double mu)
{
	if (n == 0)
		return -mu;
	return -mu + n * std::log(mu) - n * std::log(n) + n;
}

bool SingleChannelPoissonDistribution::checkConfidenceBand(double confidenceLevel) const
{
	int confidenceCutoff = getConfidenceCutoff(confidenceLevel);
	auto [min, max] = std::minmax_element(
		distribution.begin(), distribution.begin() + confidenceCutoff, Bin::lessThanCmpN
	);
	return (min->getN() < observed) && (observed < max->getN());
}

int SingleChannelPoissonDistribution::getConfidenceCutoff(double confidenceLevel) const
{
	double integratedProb = 0;
	for (int i = distribution.size() - 1; i >= 0; --i)
	{
		if (integratedProb > confidenceLevel)
		{
			return i;
		}
		integratedProb += std::exp(distribution[i].getLnLikelihoodMu());
	}
	return static_cast<int>(distribution.size() - 1);
}