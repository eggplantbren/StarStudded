#ifndef _MyModel_
#define _MyModel_

#include "Model.h"
#include <vector>
#include <RJObject.h>
#include "PSF.h"
#include "MyDistribution.h"

class MyModel:public DNest3::Model
{
	private:
		// The stars
		RJObject<MyDistribution> objects;

		// Noise standard deviation for each image
		std::vector<double> sigmas;

		// PSF model for each image
		std::vector<PSF> psfs;

		// Background levels
		std::vector<double> backgrounds;

		// The model image
		std::vector< std::vector< std::vector<double> > > images;
		void calculate_images();

	public:
		MyModel();

		// Generate the point from the prior
		void fromPrior();

		// Metropolis-Hastings proposals
		double perturb();

		// Likelihood function
		double logLikelihood() const;

		// Print to stream
		void print(std::ostream& out) const;

		// Return string with column information
		std::string description() const;
};

#endif

