#include "MyModel.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include "Data.h"
#include <cmath>

using namespace std;
using namespace DNest3;

MyModel::MyModel()
:objects(3, 100, false, MyDistribution(Data::get_instance().get_x_min() - 0.1*Data::get_instance().get_x_range(),
Data::get_instance().get_x_max() + 0.1*Data::get_instance().get_x_range(), Data::get_instance().get_y_min() - 0.1*Data::get_instance().get_y_range(), Data::get_instance().get_y_max() + 0.1*Data::get_instance().get_y_range(),
			1E-3, 1E3))
,image(Data::get_instance().get_ni(),
	vector<double>(Data::get_instance().get_nj()))
,staleness(0)
{

}

void MyModel::fromPrior()
{
	objects.fromPrior();
	calculate_image();
	sigma = exp(log(1.) + log(1E6)*randomU());
}

void MyModel::calculate_image()
{
	// Get coordinate stuff from data
	const vector< vector<double> >& x = Data::get_instance().get_x_rays();
	const vector< vector<double> >& y = Data::get_instance().get_y_rays();

	// Components
	const vector< vector<double> >& components = objects.get_components();

	// Zero the image
	image.assign(Data::get_instance().get_ni(),
					vector<double>(Data::get_instance().get_nj(), 0.));

	// Position and flux of a star
	double xx, yy, flux, rsq, exp_arg;
	double ii, jj;
	const double width = 0.05;
	const double width_in_pixels = 3.*ceil(width/Data::get_instance().get_dx());
	const double tau = 1./(width*width);
	const double C = 1./(2*M_PI*width*width);
	int imin, imax, jmin, jmax;

	for(size_t m=0; m<components.size(); m++)
	{
		xx = components[m][0];
		yy = components[m][1];
		flux = components[m][2];

		ii = (Data::get_instance().get_y_max() - yy)/
				Data::get_instance().get_dy();
		jj = (xx - Data::get_instance().get_x_min())/
				Data::get_instance().get_dx();
		
		imin = (int)floor(ii - width_in_pixels);
		imax = (int)ceil(ii + width_in_pixels);
		jmin = (int)floor(jj - width_in_pixels);
		jmax = (int)ceil(jj + width_in_pixels);
		if(imin < 0)
			imin = 0;
		if(imax > Data::get_instance().get_ni())
			imax = Data::get_instance().get_ni();
		if(jmin < 0)
			jmin = 0;
		if(jmax > Data::get_instance().get_nj())
			jmax = Data::get_instance().get_nj();

		for(int i=imin; i<imax; i++)
		{
			for(int j=jmin; j<jmax; j++)
			{
				rsq = pow(x[i][j] - xx, 2) + pow(y[i][j] - yy, 2);
				exp_arg = 0.5*tau*rsq;
				if(exp_arg < 10.)
					image[i][j] += flux*C*exp(-exp_arg);
			}
		}
	}

}

double MyModel::perturb()
{
	double logH = 0.;

	if(randomU() <= 0.75)
	{
		logH += objects.perturb();
		calculate_image();
	}
	else
	{
		sigma = log(sigma);
		sigma += log(1E6)*randh();
		sigma = mod(sigma - log(1.), log(1E6)) + log(1.);
		sigma = exp(sigma);
	}

	return logH;
}

double MyModel::logLikelihood() const
{
	const vector< vector<double> >& data = Data::get_instance().get_image();
	const vector< vector<double> >& sig = Data::get_instance().get_sigma();

	double logL = 0.;
	double var;
	for(size_t i=0; i<data.size(); i++)
	{
		for(size_t j=0; j<data[i].size(); j++)
		{
			var = sigma*sigma + sig[i][j]*sig[i][j];
			logL += -0.5*log(2.*M_PI*var)
				-0.5*pow(data[i][j] - image[i][j], 2)/var;
		}
	}

	return logL;
}

void MyModel::print(std::ostream& out) const
{
	out<<setprecision(5);
	for(size_t i=0; i<image.size(); i++)
		for(size_t j=0; j<image[i].size(); j++)
			out<<image[i][j]<<' ';
	out<<setprecision(10);
	objects.print(out); out<<' ';
	out<<sigma<<' '<<staleness<<' ';
}

string MyModel::description() const
{
	return string("objects");
}

