#include "PSF.h"
#include "Data.h"
#include "Lookup.h"
#include "DNest4/code/Utils.h"
#include <cmath>
#include <cassert>
#include <iostream>

using namespace std;
using namespace DNest4;

const double PSF::edge = 5.0;

PSF::PSF()
{

}

PSF::PSF(double sigma1, double sigma2, double weight, double q, double theta)
:sigma1(sigma1), sigma2(sigma2), weight(weight), q(q), theta(theta)
{
	sync();
}

double PSF::evaluate(double x, double y) const
{
	double xx =  x*cosTheta + y*sinTheta;
	double yy = -x*sinTheta + y*cosTheta;

	double rsq = q*xx*xx + yy*yy/q;
	double result = 0.;
	if(rsq < rsqEdge)
	{
		result = weight*preFactor1*Lookup::evaluate(0.5*rsq*pr1)
			+ (1.0 - weight)*preFactor2*Lookup::evaluate(0.5*rsq*pr2);
	}
	return result;
}

void PSF::sync()
{
	pr1 = pow(sigma1, -2);
	pr2 = pow(sigma2, -2);
	preFactor1 = pr1/(2*M_PI);
	preFactor2 = pr2/(2*M_PI);
	rEdge = PSF::edge*sigma2;
	rsqEdge = pow(rEdge, 2);
	cosTheta = cos(theta);
	sinTheta = sin(theta);
}

void PSF::from_prior(RNG& rng)
{
	// Log-uniform between 1 and 50 pixel widths
	sigma2 = exp(log(1.0) + log(50.0)*rng.rand())*Data::get_instance().get_dx();
    sigma1 = sigma2*rng.rand();

	weight = rng.rand();
	q = exp(0.3*rng.randn());
	theta = M_PI*rng.rand();

	sync();
}

double PSF::perturb(RNG& rng)
{
	double logH = 0.;

	int which = rng.rand_int(5);
	if(which == 0)
	{
        sigma1 += sigma2*rng.randh();
        wrap(sigma1, 0.0, sigma2);
	}
	else if(which == 1)
	{
        sigma1 /= sigma2;

        sigma2 = log(sigma2);
        sigma2 += log(50.0)*rng.randh();
        wrap(sigma2, log(1.0*Data::get_instance().get_dx()),
                     log(50.0*Data::get_instance().get_dx()));
        sigma2 = exp(sigma2);

        sigma1 *= sigma2;
	}
	else if(which == 2)
	{
		weight += rng.randh();
		weight = mod(weight, 1.);
	}
	else if(which == 3)
	{
		q = log(q);
        logH -= -0.5*pow(q/0.3, 2);
        q += 0.3*rng.randh();
        logH += -0.5*pow(q/0.3, 2);
        q = exp(q);
	}
	else
	{
		theta += M_PI*rng.randh();
		wrap(theta, 0., M_PI);
	}

	sync();
	return logH;
}

ostream& operator << (ostream& out, const PSF& psf)
{
	out<<psf.sigma1<<' '<<psf.sigma2<<' '<<psf.weight<<' '<<psf.q;
	out<<' '<<psf.theta<<' ';
	return out;
}

