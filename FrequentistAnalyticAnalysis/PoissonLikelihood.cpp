#include "PoissonLikelihood.h"
#include <cmath>
#include <stdexcept>
const double PoissonLikelihood::lnZero = 100, PoissonLikelihood::lnError = 1000;
PoissonLikelihood::PoissonLikelihood(int n, double mu)
{
	updateLikelihood(n, mu);
}

PoissonLikelihood::PoissonLikelihood()
{
	lnLikelihood = lnError;
}

bool PoissonLikelihood::operator<(const PoissonLikelihood& other) const
{
	if (lnLikelihood == lnZero && other.lnLikelihood == lnZero)
		return false;
	if (lnLikelihood == lnZero)
		return true;
	if (other.lnLikelihood == lnZero)
		return false;
	return lnLikelihood < other.lnLikelihood;
}


bool PoissonLikelihood::operator>(const PoissonLikelihood& other) const
{
	if (lnLikelihood == lnZero && other.lnLikelihood == lnZero)
		return false;
	if (lnLikelihood == lnZero)
		return false;
	if (other.lnLikelihood == lnZero)
		return true;
	return lnLikelihood > other.lnLikelihood;
}

double PoissonLikelihood::toLnLikelihood() const
{
	if (lnLikelihood == lnZero)
		throw std::domain_error("Invalid ln value in PoissonLikelihood::toLnLikelihood().");
	return lnLikelihood;
}

double PoissonLikelihood::lnRatio(const PoissonLikelihood& other) const
{
	if (other.lnLikelihood == lnZero)
		throw std::domain_error("Invalid denominator in PoissonLikelihood::lnRatio().");
	return lnLikelihood - other.lnLikelihood;
}

void PoissonLikelihood::updateLikelihood(int n, double mu)
{
	if (mu == 0)
		lnLikelihood = (n == 0) ? 0 : lnZero;
	lnLikelihood = n * std::log(mu) - mu - std::lgamma(n + 1);
}

double PoissonLikelihood::toLikelihood() const
{
	return (lnLikelihood == lnZero) ? 0 : std::exp(lnLikelihood);
}
