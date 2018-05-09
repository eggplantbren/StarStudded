#include "MyModel.h"
#include "DNest4/code/Utils.h"
#include "Data.h"
#include <cmath>
#include <sstream>
#include "Assumptions.h"
#include "Lookup.h"

using namespace std;
using namespace DNest4;
using namespace StarStudded;

MyModel::MyModel()
:max_num_stars(Assumptions::get_instance().get_max_num_stars())
,padding(Assumptions::get_instance().get_padding())
,objects(2 + Data::get_instance().get_num_images(),
         max_num_stars,
         false,
         MyConditionalPrior(Data::get_instance().get_x_min() -
               padding*Data::get_instance().get_x_range(),
Data::get_instance().get_x_max() + padding*Data::get_instance().get_x_range(), Data::get_instance().get_y_min() - padding*Data::get_instance().get_y_range(), Data::get_instance().get_y_max() + padding*Data::get_instance().get_y_range()), PriorType::log_uniform)
,sigmas0(Data::get_instance().get_num_images())
,sigmas1(Data::get_instance().get_num_images())
,psfs(Data::get_instance().get_num_images())
,backgrounds(Data::get_instance().get_num_images())
,signs(Data::get_instance().get_num_images())
,images(Data::get_instance().get_num_images(), vector< vector<double> >(Data::get_instance().get_ni(),
					vector<double>(Data::get_instance().get_nj())))
{

}

void MyModel::from_prior(RNG& rng)
{
	objects.from_prior(rng);
	for(size_t i=0; i<psfs.size(); i++)
		psfs[i].from_prior(rng);
	calculate_images(false);

	for(size_t i=0; i<backgrounds.size(); ++i)
    {
		backgrounds[i] = exp(tan(M_PI*(0.97*rng.rand() - 0.485)));
        signs[i] = (rng.rand() <= 0.5)?(1):(-1);
    }

	for(size_t i=0; i<sigmas0.size(); ++i)
		sigmas0[i] = exp(tan(M_PI*(0.97*rng.rand() - 0.485)));
	for(size_t i=0; i<sigmas1.size(); ++i)
		sigmas1[i] = exp(tan(M_PI*(0.97*rng.rand() - 0.485)));
}

void MyModel::calculate_image(int img, bool update)
{
    // Get coordinate stuff from data
	const vector< vector<double> >& x = Data::get_instance().get_x_rays();
	const vector< vector<double> >& y = Data::get_instance().get_y_rays();

	// Components
	const vector< vector<double> >& components = (update)?
                                                 (objects.get_added()):
                                                 (objects.get_components());

	// Zero the image
    if(!update)
    {
	    images[img].assign(Data::get_instance().get_ni(),
					       vector<double>(Data::get_instance().get_nj(), 0.0));
    }

	// Position and flux of a star
	double xx, yy, flux;
	double ii, jj;

    double width = psfs[img].get_sigma2();
	double width_in_pixels = PSF::edge*ceil(width/Data::get_instance().get_dx());
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
				images[img][i][j] += flux*psfs[img].evaluate(x[i][j] - xx,
															y[i][j] - yy);
			}
		}
	}
}

void MyModel::calculate_images(bool update)
{
    for(size_t i=0; i<images.size(); ++i)
        calculate_image(i, update);
}

double MyModel::perturb(RNG& rng)
{
	double logH = 0.;

	if(rng.rand() <= 0.5)
	{
		logH += objects.perturb(rng);

        // Pre-reject, incorporating prior here
        if(rng.rand() >= exp(logH))
            return -1E300;
        else
            logH = 0.0;

		calculate_images(objects.get_removed().size() == 0);
	}
	else
	{
		int what = rng.rand_int(3);
		if(what == 0)
		{
			int which = rng.rand_int(psfs.size());
			logH += psfs[which].perturb(rng);

            // Pre-reject, incorporating prior here
            if(rng.rand() >= exp(logH))
                return -1E300;
            else
                logH = 0.0;

			calculate_image(which, false);
		}
		if(what == 1)
		{
			int which = rng.rand_int(sigmas0.size());

			sigmas0[which] = log(sigmas0[which]);
			sigmas0[which] = (atan(sigmas0[which])/M_PI + 0.485)/0.97;
			sigmas0[which] += rng.randh();
			wrap(sigmas0[which], 0., 1.);
			sigmas0[which] = tan(M_PI*(0.97*sigmas0[which] - 0.485));
			sigmas0[which] = exp(sigmas0[which]);

			sigmas1[which] = log(sigmas1[which]);
			sigmas1[which] = (atan(sigmas1[which])/M_PI + 0.485)/0.97;
			sigmas1[which] += rng.randh();
			wrap(sigmas1[which], 0., 1.);
			sigmas1[which] = tan(M_PI*(0.97*sigmas1[which] - 0.485));
			sigmas1[which] = exp(sigmas1[which]);
		}
		if(what == 2)
		{
			int which = rng.rand_int(backgrounds.size());

			backgrounds[which] = log(backgrounds[which]);
			backgrounds[which] = (atan(backgrounds[which])/M_PI + 0.485)/0.97;
			backgrounds[which] += rng.randh();
			wrap(backgrounds[which], 0., 1.);
			backgrounds[which] = tan(M_PI*(0.97*backgrounds[which] - 0.485));
			backgrounds[which] = exp(backgrounds[which]);

            // Possible sign flip
            if(rng.rand() <= 0.5)
                signs[which] *= -1;
		}
	}

	return logH;
}

double MyModel::log_likelihood() const
{
	const vector< vector< vector<double> > >& data = Data::get_instance().get_images();
	const vector< vector< vector<double> > >& sig = Data::get_instance().get_sigmas();

	double logL = 0.;
	double bg, var;

	for(int img=0; img<Data::get_instance().get_num_images(); img++)
	{
        bg = backgrounds[img]*signs[img];

		for(int i=0; i<Data::get_instance().get_ni(); i++)
		{
			for(int j=0; j<Data::get_instance().get_nj(); j++)
			{
				var = sig[img][i][j]*sig[img][i][j]
                        + sigmas0[img]*sigmas0[img]
                        + sigmas1[img]*images[img][i][j];

				logL += -0.5*log(2.*M_PI*var)
					-0.5*pow(data[img][i][j] - (bg + images[img][i][j]), 2)/var;
			}
		}
	}

	return logL;
}

void MyModel::print(std::ostream& out) const
{
	out<<setprecision(6);

    double bg;
	for(int img=0; img<Data::get_instance().get_num_images(); img++)
    {
        bg = backgrounds[img]*signs[img];
		for(int i=0; i<Data::get_instance().get_ni(); i++)
			for(int j=0; j<Data::get_instance().get_nj(); j++)
				out<<(bg + images[img][i][j])<<' ';
    }
	out<<setprecision(10);
	objects.print(out); out<<' ';
	for(int img=0; img<Data::get_instance().get_num_images(); ++img)
		out<<sigmas0[img]<<' '<<sigmas1[img]<<' ';

    for(int img=0; img<Data::get_instance().get_num_images(); ++img)
    {
        bg = backgrounds[img]*signs[img];
        out<<bg<<' ';
    }
}

string MyModel::description() const
{
    stringstream s;
    for(int img=0; img<Data::get_instance().get_num_images(); img++)
    {
        for(int i=0; i<Data::get_instance().get_ni(); i++)
            for(int j=0; j<Data::get_instance().get_nj(); j++)
                s<<"model_image["<<img<<"]["<<i<<"]["<<j<<"], ";
    }
    s<<"num_dimensions_star, max_num_stars, ";

    for(int img=0; img<Data::get_instance().get_num_images(); img++)
        s<<"typical_fluxes["<<img<<"], sig_log_fluxes["<<img<<"], ";

    s<<"num_stars, ";

    for(size_t i=0; i<max_num_stars; ++i)
        s<<"star_x["<<i<<"], ";
    for(size_t i=0; i<max_num_stars; ++i)
        s<<"star_y["<<i<<"], ";
    for(int img=0; img<Data::get_instance().get_num_images(); img++)
        for(size_t i=0; i<max_num_stars; ++i)
            s<<"star_flux["<<img<<"]["<<i<<"], ";

    for(int img=0; img<Data::get_instance().get_num_images(); ++img)
        s<<"sigma0["<<img<<"], sigma1["<<img<<"], ";

    for(int img=0; img<Data::get_instance().get_num_images(); ++img)
        s<<"bg["<<img<<"], ";

    return s.str();
}

