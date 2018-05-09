#include "MyConditionalPrior.h"
#include "Data.h"
#include "DNest4/code/Distributions/Cauchy.h"
#include "DNest4/code/Distributions/Gaussian.h"
#include "DNest4/code/Utils.h"
#include <cmath>

using namespace DNest4;

namespace StarStudded
{

// Alias
using Normal = DNest4::Gaussian;

MyConditionalPrior::MyConditionalPrior(double x_min, double x_max,
					                   double y_min, double y_max)
:typical_fluxes(Data::get_instance().get_num_images())
,sig_log_fluxes(Data::get_instance().get_num_images())
,x_min(x_min)
,x_max(x_max)
,y_min(y_min)
,y_max(y_max)
{

}

void MyConditionalPrior::from_prior(DNest4::RNG& rng)
{
    // A Cauchy distribution
    DNest4::Cauchy cauchy(0.0, 5.0);

	for(int img=0; img<Data::get_instance().get_num_images(); img++)
	{
        // Log-Cauchy with truncation
        while(true)
        {
    		typical_fluxes[img] = cauchy.generate(rng);
            if(std::abs(typical_fluxes[img]) < 100.0)
                break;
        }
        typical_fluxes[img] = exp(typical_fluxes[img]);

        // Uniform
        sig_log_fluxes[img] = 3*rng.rand();
	}
}

double MyConditionalPrior::perturb_hyperparameters(DNest4::RNG& rng)
{
	double logH = 0.0;

	int which = rng.rand_int(2);
	int index = rng.rand_int(Data::get_instance().get_num_images());

	if(which == 0)
	{
        // A Cauchy distribution
        DNest4::Cauchy cauchy(0.0, 5.0);

        typical_fluxes[index] = log(typical_fluxes[index]);
        logH += cauchy.perturb(typical_fluxes[index], rng);

        // Handle truncation
        if(std::abs(typical_fluxes[index]) >= 100.0)
        {
            typical_fluxes[index] = 1.0;
            return -1E300;
        }

        typical_fluxes[index] = exp(typical_fluxes[index]);
	}
	else if(which == 1)
	{
		sig_log_fluxes[index] += 3*rng.randh();
		sig_log_fluxes[index] = DNest4::mod(sig_log_fluxes[index], 3.0);
	}

	return logH;
}

// vec = {x, y, fluxes}
double MyConditionalPrior::log_pdf(const std::vector<double>& vec) const
{
    // Check hard limits
	if(vec[0] < x_min || vec[0] > x_max || vec[1] < y_min || vec[1] > y_max)
		return -1E300;
	for(int img=0; img<Data::get_instance().get_num_images(); img++)
		if(vec[img+2] < 0.0)
			return -1E300;


	double logp = 0.0;
	for(int img=0; img<Data::get_instance().get_num_images(); img++)
	{
        // A normal distribution for log-flux
        Normal normal(log(typical_fluxes[img]), sig_log_fluxes[img]);

        double log_flux = log(vec[img+2]);
        logp += normal.log_pdf(log_flux) - log_flux;
	}

	return logp;
}

void MyConditionalPrior::from_uniform(std::vector<double>& vec) const
{
	vec[0] = x_min + (x_max - x_min)*vec[0];
	vec[1] = y_min + (y_max - y_min)*vec[1];

	for(int img=0; img<Data::get_instance().get_num_images(); img++)
	{
        // A normal distribution for log-flux
        Normal normal(log(typical_fluxes[img]), sig_log_fluxes[img]);
        vec[img+2] = exp(normal.cdf_inverse(vec[img+2]));
	}

}

void MyConditionalPrior::to_uniform(std::vector<double>& vec) const
{
	vec[0] = (vec[0] - x_min)/(x_max - x_min);
	vec[1] = (vec[1] - y_min)/(y_max - y_min);

	for(int img=0; img<Data::get_instance().get_num_images(); img++)
	{
        // A normal distribution for log-flux
        Normal normal(log(typical_fluxes[img]), sig_log_fluxes[img]);
        vec[img+2] = normal.cdf(log(vec[img+2]));
	}
}

void MyConditionalPrior::print(std::ostream& out) const
{
	for(int img=0; img<Data::get_instance().get_num_images(); img++)
		out << typical_fluxes[img] << ' ' << sig_log_fluxes[img] << ' ';
}

} // namespace StarStudded

