#ifndef _MyDistribution_
#define _MyDistribution_

#include <Distributions/Distribution.h>

// Hyperparameters setting conditional prior for star properties
class MyDistribution:public Distribution
{
	private:
		// Pareto prior for the fluxes
		// One for each band
		std::vector<double> fluxlim;
		std::vector<double> gamma;

		// Save limits
		double x_min, x_max, y_min, y_max, fluxlim_min, fluxlim_max;

		double perturb_parameters();

	public:
		MyDistribution(double x_min, double x_max,
					double y_min, double y_max,
					double fluxlim_min, double fluxlim_max);

		void fromPrior();

		double log_pdf(const std::vector<double>& vec) const;
		void from_uniform(std::vector<double>& vec) const;
		void to_uniform(std::vector<double>& vec) const;

		void print(std::ostream& out) const;
};

#endif

