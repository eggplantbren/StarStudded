#include "MyModel.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include "Data.h"
#include <cmath>
#include "Lookup.h"

using namespace std;
using namespace DNest3;

MyModel::MyModel()
:objects(2 + Data::get_instance().get_num_images(), 100, false, MyDistribution(Data::get_instance().get_x_min() - 0.1*Data::get_instance().get_x_range(),
Data::get_instance().get_x_max() + 0.1*Data::get_instance().get_x_range(), Data::get_instance().get_y_min() - 0.1*Data::get_instance().get_y_range(), Data::get_instance().get_y_max() + 0.1*Data::get_instance().get_y_range(),
			1E-3, 1E3))
,images(Data::get_instance().get_num_images(), vector< vector<double> >(Data::get_instance().get_ni(),
					vector<double>(Data::get_instance().get_nj())))
,sigmas(Data::get_instance().get_num_images())
{

}

void MyModel::fromPrior()
{
	objects.fromPrior();
	calculate_images();
	for(size_t i=0; i<sigmas.size(); i++)
		sigmas[i] = exp(log(1.) + log(1E6)*randomU());
}

void MyModel::calculate_images()
{
	// Get coordinate stuff from data
	const vector< vector<double> >& x = Data::get_instance().get_x_rays();
	const vector< vector<double> >& y = Data::get_instance().get_y_rays();

	// Components
	const vector< vector<double> >& components = objects.get_components();

	// Zero the image
	images.assign(Data::get_instance().get_num_images(), vector< vector<double> >(Data::get_instance().get_ni(),
					vector<double>(Data::get_instance().get_nj(), 0.)));

	// Position and flux of a star
	double xx, yy, flux, rsq, exp_arg;
	double ii, jj;

	for(int img=0; img<Data::get_instance().get_num_images(); img++)
	{
		const double width = 0.05;
		const double width_in_pixels = 3.*ceil(width/Data::get_instance().get_dx());
		const double tau = 1./(width*width);
		const double C = 1./(2*M_PI*width*width);
		int imin, imax, jmin, jmax;

		for(size_t m=0; m<components.size(); m++)
		{
			xx = components[m][0];
			yy = components[m][1];
			flux = components[m][2 + img];

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
						images[img][i][j] += flux*C*Lookup::evaluate(exp_arg);
				}
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
		calculate_images();
	}
	else
	{
		int which = randInt(sigmas.size());

		sigmas[which] = log(sigmas[which]);
		sigmas[which] += log(1E6)*randh();
		sigmas[which] = mod(sigmas[which] - log(1.), log(1E6)) + log(1.);
		sigmas[which] = exp(sigmas[which]);
	}

	return logH;
}

double MyModel::logLikelihood() const
{
	const vector< vector< vector<double> > >& data = Data::get_instance().get_images();
	const vector< vector< vector<double> > >& sig = Data::get_instance().get_sigmas();

	double logL = 0.;
	double var;

	for(int img=0; img<Data::get_instance().get_num_images(); img++)
	{
		for(int i=0; i<Data::get_instance().get_ni(); i++)
		{
			for(int j=0; j<Data::get_instance().get_nj(); j++)
			{
				var = sigmas[img]*sigmas[img] + sig[img][i][j]*sig[img][i][j];
				logL += -0.5*log(2.*M_PI*var)
					-0.5*pow(data[img][i][j] - images[img][i][j], 2)/var;
			}
		}
	}

	return logL;
}

void MyModel::print(std::ostream& out) const
{
	out<<setprecision(5);

	for(int img=0; img<Data::get_instance().get_num_images(); img++)
		for(int i=0; i<Data::get_instance().get_ni(); i++)
			for(int j=0; j<Data::get_instance().get_nj(); j++)
				out<<images[img][i][j]<<' ';
	out<<setprecision(10);
	objects.print(out); out<<' ';
	for(int img=0; img<Data::get_instance().get_num_images(); img++)
		out<<sigmas[img]<<' ';
}

string MyModel::description() const
{
	return string("objects");
}

