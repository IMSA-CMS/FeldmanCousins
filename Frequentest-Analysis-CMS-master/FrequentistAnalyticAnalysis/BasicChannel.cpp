#include "BasicChannel.h"
BasicChannel::BasicChannel(double iConstCoeff, double iLinCoeff,
	double iSqCoeff, double iBgOne, double iBgTwo, int iObserved)
	: constCoeff(iConstCoeff),
	linCoeff(iLinCoeff),
	sqCoeff(iSqCoeff),
	bgOne(iBgOne),
	bgTwo(iBgTwo),
	observed(iObserved)
{ }

double BasicChannel::getMu(double beta) const
{
	return constCoeff + linCoeff * beta + sqCoeff * beta * beta + bgOne + bgTwo;
}
