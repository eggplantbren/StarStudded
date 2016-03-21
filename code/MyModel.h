#ifndef _MyModel_
#define _MyModel_

#include <vector>
#include "PSF.h"
#include "DNest4/code/RJObject/RJObject.h"
#include "MyConditionalPrior.h"

class MyModel
{
	private:
		// The stars
		DNest4::RJObject<MyConditionalPrior> objects;

		// Noise standard deviation for each image
		std::vector<double> sigmas;

		// PSF model for each image
		std::vector<PSF> psfs;

		// Background levels
		std::vector<double> backgrounds;
        std::vector<short> signs;

		// The model image
		std::vector< std::vector< std::vector<double> > > images;

        /****** Private member functions ******/
        void calculate_image(int img);
		void calculate_images();

	public:
		MyModel();

		// Generate the point from the prior
		void from_prior(DNest4::RNG& rng);

		// Metropolis-Hastings proposals
		double perturb(DNest4::RNG& rng);

		// Likelihood function
		double log_likelihood() const;

		// Print to stream
		void print(std::ostream& out) const;

		// Return string with column information
		std::string description() const;
};

#endif

