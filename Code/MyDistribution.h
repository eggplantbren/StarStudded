#ifndef _MyDistribution_
#define _MyDistribution_

#include <Distributions/Distribution.h>

// Hyperparameters setting interim prior for galaxy properties
class MyDistribution:public Distribution
{
	private:
		// Limits
		double x_min, x_max, y_min, y_max;

		// Lower limit and 1/slope for Pareto interim prior
		// for fluxes
		double min_logflux;
		double gamma;

		double perturb_parameters();

	public:
		MyDistribution(double x_min, double x_max,
					double y_min, double y_max);
		void fromPrior();

		double log_pdf(const std::vector<double>& vec) const;
		void from_uniform(std::vector<double>& vec) const;
		void to_uniform(std::vector<double>& vec) const;

		void print(std::ostream& out) const;
};

#endif

