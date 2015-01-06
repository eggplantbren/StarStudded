#include "MyDistribution.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include <cmath>

using namespace DNest3;

MyDistribution::MyDistribution(double x_min, double x_max,
					double y_min, double y_max)
:x_min(x_min)
,x_max(x_max)
,y_min(y_min)
,y_max(y_max)
{

}

void MyDistribution::fromPrior()
{
	min_logflux = tan(M_PI*(0.97*randomU() - 0.485));
	gamma = 0.1 + 2.9*randomU();
}

double MyDistribution::perturb_parameters()
{
	double logH = 0.;

	int which = randInt(2);

	if(which == 0)
	{
		min_logflux = (atan(min_logflux)/M_PI + 0.485)/0.97;
		min_logflux += randh();
		wrap(min_logflux, 0., 1.);
		min_logflux = tan(M_PI*(0.97*min_logflux - 0.485));
	}
	if(which == 1)
	{
		gamma += 2.9*randh();
		wrap(gamma, 0.1, 3.);
	}

	return logH;
}

// x, y, logflux
double MyDistribution::log_pdf(const std::vector<double>& vec) const
{
	double logp = 0.;

	if(vec[0] < x_min || vec[0] > x_max || vec[1] < y_min || vec[1] > y_max
		|| vec[2] < min_logflux)
		return -1E300;

	return logp;
}

void MyDistribution::from_uniform(std::vector<double>& vec) const
{
	vec[0] = x_min + (x_max - x_min)*vec[0];
	vec[1] = y_min + (y_max - y_min)*vec[1];
	vec[2] = min_logflux - gamma*log(1. - vec[2]);
}

void MyDistribution::to_uniform(std::vector<double>& vec) const
{
	vec[0] = (vec[0] - x_min)/(x_max - x_min);
	vec[1] = (vec[1] - y_min)/(y_max - y_min);
	vec[2] = 1. - exp(-(vec[2] - min_logflux)/gamma);
}

void MyDistribution::print(std::ostream& out) const
{
	out<<min_logflux<<' '<<gamma<<' ';
}

