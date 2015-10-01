#include "MyModel.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include "Data.h"
#include <cmath>

using namespace std;
using namespace DNest3;

MyModel::MyModel()
:objects(3, 100, false, MyDistribution(
	Data::get_instance().get_x_min(), Data::get_instance().get_x_max(),
	Data::get_instance().get_y_min(), Data::get_instance().get_y_max(),
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
	double xx, yy, flux, rsq;
	const double width = 0.05;
	const double tau = 1./(width*width);
	double C = 1./(2*M_PI*width*width);

	for(size_t m=0; m<components.size(); m++)
	{
		xx = components[m][0];
		yy = components[m][1];
		flux = components[m][2];

		for(size_t i=0; i<image.size(); i++)
		{
			for(size_t j=0; j<image[i].size(); j++)
			{
				rsq = pow(x[i][j] - xx, 2) + pow(y[i][j] - yy, 2);
				image[i][j] += flux*C*exp(-0.5*tau*rsq);
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

