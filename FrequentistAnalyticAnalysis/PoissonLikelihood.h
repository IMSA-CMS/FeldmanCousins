#ifndef FREQUENTIST_ANALYTIC_ANALYSIS_POISSONLIKELIHOOD_H
#define FREQUENTIST_ANALYTIC_ANALYSIS_POISSONLIKELIHOOD_H
// PoissonLikelihood.h: Stores the likelihood value.
// Abstracts a lot of the details about how the likelihood should be handled.
// Stores the likelihood values as logs, and handles the wierd quirks that come along
// with storing the likelihood values as such.
class PoissonLikelihood
{
	public:
		//Calculates the ln(poisson likelihood P(n|mu))
		//To normalize distribution special cases are needed
		//values greater than 0 are non-normal lnLikelihood values which represent 0 likelihood.
		PoissonLikelihood(int n, double mu);

		//Default constructor (initializes in error state)
		PoissonLikelihood();

		//Comparisons for PoissonLikelihood that handle special cases properly
		bool operator<(const PoissonLikelihood& other) const;
		bool operator>(const PoissonLikelihood& other) const;
		

		//Converts PoissonLikelihood to a likelihood
		double toLikelihood() const;

		//Converts PoissonLikelihood to a logLikelihood
		//Throws if you try to take the log of zero.
		double toLnLikelihood() const;

		double lnRatio(const PoissonLikelihood& other) const;

		//Update likelihood
		void updateLikelihood(int n, double mu);
	private:
		double lnLikelihood;
		static const double lnZero, lnError;
		
};
#endif