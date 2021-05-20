#include "PoissonBin.h"
#include <cmath>
PoissonBin::PoissonBin(int iN, double iMu)
	: n(iN),
	lnLikelihoodMu(calculateLnPoissonLikelihood(iN,iMu)),
	lnLikelihoodN(calculateLnPoissonLikelihood(iN,iN))
{ }

bool PoissonBin::lessThanCmpFeldmanCousins(const PoissonBin& one, const PoissonBin& two)
{
	return (one.lnLikelihoodMu - one.lnLikelihoodN) > (two.lnLikelihoodMu - two.lnLikelihoodN);
}

bool PoissonBin::lessThanCmpN(const PoissonBin& one, const PoissonBin& two)
{
	return one.n < two.n;
}

double PoissonBin::calculateLnPoissonLikelihood(int n, double mu)
{
	if (n == 0)
		return -mu;
	return -mu + n * std::log(mu) - n * std::log(n) + n;
}