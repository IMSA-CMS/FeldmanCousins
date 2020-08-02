#pragma once
#include <vector>
#include <cmath>
#include "Coordinate.h"
class Bin
{
	public:
		struct NMuPair
		{
			int n;
			double mu;
			NMuPair(int iN, double iMu) : n(iN), mu(iMu) {};
		};
		//Build a bin out of n and mu pairs.
		Bin(std::vector<NMuPair> nMuPairs);

		//Gets the N for the channel index
		int getN(int channelIndex) const;

		//Gets the likelihood for the channel index
		double getLnLikelihood(int channelIndex) const;

		//Gets the total likelihood for the bin
		double getTotalLnLikelihood() const;

		//Gets the number of dimensions
		int getDimensions() const { return dimensions; }

		//Compares two Bins using the FeldmanCousins method
		bool operator<(const Bin& other) const;

		//Normalizes the bin with the given normalization factor
		//(usually the maximum likelihood)
		void normalize(const Coordinate<double>& normalizationFactor);

		//Gets the coordinate for all of the N's
		const Coordinate<int>& getAllN() const;
	private:
		int dimensions;
		Coordinate<int> nArray;
		Coordinate<double> likelihoodArray;

		//Calculates the ln(poisson likelihood P(n|mu))
		static double calculateLnPoissonLikelihood(int n, double mu);
};