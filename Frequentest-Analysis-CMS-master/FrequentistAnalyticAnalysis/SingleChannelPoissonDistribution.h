#pragma once
#include <utility>
#include <vector>
#include <algorithm>
// An analytic, single-bin version of FirstAnalyticParticleGraph.h (see FirstAnalyticParticleGraph.h for more details).
// To be primarily for debugging (but honestly do whatever)
class SingleChannelPoissonDistribution
{
	public:
		//Builds SingleChannelPoissonDistribution using a channel and a beta.
		//ChannelReader must have the subscript operator overloaded that returns a channel
		//Which has methods getMu and getObserved
		//As well as a method that returns the number of channels read called getChannelsRead()
		template<typename ChannelReader>
		SingleChannelPoissonDistribution(ChannelReader& channelReader, double iBeta, int nUpper = nUpperDefault);

		//Checks confidence band for a given confidence level.
		bool checkConfidenceBand(double confidenceLevel) const;
	private:
		//Bin helper class
		class Bin
		{
		public:
			//Builds a Bin from an N and a mu.
			Bin(int iN, double iMu);

			//Gets the poisson likelihood calculated with mu.
			double getLnLikelihoodMu() const { return lnLikelihoodMu; }

			//Gets the likelihood calculated with the observed number of events.
			double getLnLikelihoodN() const { return lnLikelihoodN; }

			//Gets the n used to calculate the likelihoods.
			double getN() const { return n; }

			//Do a less-than comparison using the Feldman-Cousins method.
			static bool lessThanCmpFeldmanCousins(const Bin& one, const Bin& two);

			//Do a less-than comparison on N.
			static bool lessThanCmpN(const Bin& one, const Bin& two);
		private:
			int n;
			double lnLikelihoodMu, lnLikelihoodN;

			//Calculates the ln(poisson likelihood P(n|mu))
			static inline double calculateLnPoissonLikelihood(int n, double mu);
		};

		static int constexpr nUpperDefault = 30000;
		std::vector<Bin> distribution;
		int observed;

		//Gets the cutoff position for the confidence band created with the specified confidence level.
		int getConfidenceCutoff(double confidenceLevel) const;
};

template<typename ChannelReader>
SingleChannelPoissonDistribution::SingleChannelPoissonDistribution(ChannelReader& channelReader,
	double iBeta, int nUpper)
	: observed(channelReader[0].getObserved())
{
	distribution.reserve(nUpper);
	for (int i = 0; i < nUpper; ++i)
		distribution.emplace_back(i, channelReader[0].getMu(iBeta));
	std::sort(distribution.begin(), distribution.end(), Bin::lessThanCmpFeldmanCousins);
}