#ifndef StarField_MyConditionalPrior_h
#define StarField_MyConditionalPrior_h

#include "DNest4/code/RJObject/ConditionalPriors/ConditionalPrior.h"

// Hyperparameters setting conditional prior for star properties
class MyConditionalPrior:public DNest4::ConditionalPrior
{
	private:

		// Lognormal prior for stellar fluxes - one for each band
        std::vector<double> typical_fluxes;
        std::vector<double> sig_log_fluxes;

		// Save image limits
		double x_min, x_max, y_min, y_max;

		double perturb_hyperparameters(DNest4::RNG& rng);

	public:
		MyConditionalPrior(double x_min, double x_max,
					       double y_min, double y_max);

		void from_prior(DNest4::RNG& rng);

		double log_pdf(const std::vector<double>& vec) const;
		void from_uniform(std::vector<double>& vec) const;
		void to_uniform(std::vector<double>& vec) const;

		void print(std::ostream& out) const;
};

#endif

