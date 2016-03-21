#include "MyConditionalPrior.h"
#include "DNest4/code/Utils.h"
#include <cmath>
#include "Data.h"

using namespace DNest4;

MyConditionalPrior::MyConditionalPrior(double x_min, double x_max,
					                   double y_min, double y_max)
:fluxlim(Data::get_instance().get_num_images())
,gamma(Data::get_instance().get_num_images())
,x_min(x_min)
,x_max(x_max)
,y_min(y_min)
,y_max(y_max)
{

}

void MyConditionalPrior::from_prior(DNest4::RNG& rng)
{
	for(int img=0; img<Data::get_instance().get_num_images(); img++)
	{
		fluxlim[img] = exp(tan(M_PI*(0.97*rng.rand() - 0.485)));
		gamma[img] = 3*rng.rand();
	}
}

double MyConditionalPrior::perturb_hyperparameters(DNest4::RNG& rng)
{
	double logH = 0.;

	int which = rng.rand_int(2);
	int index = rng.rand_int(Data::get_instance().get_num_images());

	if(which == 0)
	{
		fluxlim[index] = log(fluxlim[index]);
		fluxlim[index] = (atan(fluxlim[index])/M_PI + 0.485)/0.97;
		fluxlim[index] += rng.randh();
		wrap(fluxlim[index], 0.0, 1.0);
		fluxlim[index] = tan(M_PI*(0.97*fluxlim[index] - 0.485));
		fluxlim[index] = exp(fluxlim[index]);
	}
	else if(which == 1)
	{
		gamma[index] += 3*rng.randh();
		gamma[index] = mod(gamma[index], 3.0);
	}

	return logH;
}

// x, y, flux
double MyConditionalPrior::log_pdf(const std::vector<double>& vec) const
{

	if(vec[0] < x_min || vec[0] > x_max || vec[1] < y_min || vec[1] > y_max)
		return -1E300;
	for(int img=0; img<Data::get_instance().get_num_images(); img++)
		if(vec[2+img] < fluxlim[img])
			return -1E300;

	double logp = 0.;

	double alpha;
	for(int img=0; img<Data::get_instance().get_num_images(); img++)
	{
		alpha = 1./gamma[img];
		logp += log(alpha) + alpha*log(fluxlim[img]) - (alpha + 1.)*log(vec[2 + img]);
	}

	return logp;
}

void MyConditionalPrior::from_uniform(std::vector<double>& vec) const
{
	vec[0] = x_min + (x_max - x_min)*vec[0];
	vec[1] = y_min + (y_max - y_min)*vec[1];

	for(int img=0; img<Data::get_instance().get_num_images(); img++)
		vec[2+img] = fluxlim[img]*pow(1. - vec[2+img], -gamma[img]);
}

void MyConditionalPrior::to_uniform(std::vector<double>& vec) const
{
	vec[0] = (vec[0] - x_min)/(x_max - x_min);
	vec[1] = (vec[1] - y_min)/(y_max - y_min);

	double alpha;
	for(int img=0; img<Data::get_instance().get_num_images(); img++)
	{
		alpha = 1./gamma[img];
		vec[2 + img] = 1. - pow(fluxlim[img]/vec[2 + img], alpha);
	}
}

void MyConditionalPrior::print(std::ostream& out) const
{
	for(int img=0; img<Data::get_instance().get_num_images(); img++)
		out<<fluxlim[img]<<' '<<gamma[img]<<' ';
}

