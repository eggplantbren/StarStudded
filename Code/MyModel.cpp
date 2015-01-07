#include "MyModel.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include "Data.h"
#include <cmath>

using namespace std;
using namespace DNest3;

MyModel::MyModel()
:objects(3, 200, false, MyDistribution(
	Data::get_instance().get_x_min(), Data::get_instance().get_x_max(),
	Data::get_instance().get_y_min(), Data::get_instance().get_y_max()))
,image(Data::get_instance().get_ni(),
	vector<long double>(Data::get_instance().get_nj()))
{

}

void MyModel::fromPrior()
{
	objects.fromPrior();
	objects.consolidate_diff();
	calculate_image();

	sigma = exp(tan(M_PI*(0.97*randomU() - 0.485)));
}

void MyModel::calculate_image()
{
	// Get coordinate stuff from data
	const vector< vector<double> >& x = Data::get_instance().get_x_rays();
	const vector< vector<double> >& y = Data::get_instance().get_y_rays();

	// Components
	const vector< vector<double> >& components = objects.get_components();
}

double MyModel::perturb()
{
	double logH = 0.;

	if(randomU() <= 0.75)
	{
		logH += objects.perturb();
		objects.consolidate_diff();
		calculate_image();
	}
	else
	{
		sigma = log(sigma);
		sigma = (atan(sigma)/M_PI + 0.485)/0.97;
		sigma += randh();
		wrap(sigma, 0., 1.);
		sigma = tan(M_PI*(0.97*sigma - 0.485));
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
	out<<sigma<<' ';
}

string MyModel::description() const
{
	return string("objects");
}

