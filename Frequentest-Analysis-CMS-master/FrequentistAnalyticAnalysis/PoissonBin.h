#pragma once
class PoissonBin
{
	public:
		//Builds a PoissonBin from an N, mu, and observed number of events.
		PoissonBin(int iN, double iMu);

		//Gets the poisson likelihood calculated with mu.
		double getLnLikelihoodMu() const { return lnLikelihoodMu; }

		//Gets the likelihood calculated with the observed number of events.
		double getLnLikelihoodN() const { return lnLikelihoodN; }

		//Gets the n used to calculate the likelihoods.
		double getN() const { return n; }

		//Do a less-than comparison using the Feldman-Cousins method.
		static bool lessThanCmpFeldmanCousins(const PoissonBin& one, const PoissonBin& two);

		//Do a less-than comparison on N.
		static bool lessThanCmpN(const PoissonBin& one, const PoissonBin& two);
	private:
		int n;
		double lnLikelihoodMu, lnLikelihoodN;

		//Calculates the ln(poisson likelihood P(n|mu))
		static inline double calculateLnPoissonLikelihood(int n, double mu);
};
