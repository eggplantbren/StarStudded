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
        // sigma1 = constant level, sigma2 = scale with image
		std::vector<double> sigmas0;
        std::vector<double> sigmas1;

		// PSF model for each image
		std::vector<PSF> psfs;

		// Background levels
		std::vector<double> backgrounds;
        std::vector<short> signs;

		// The model image
		std::vector< std::vector< std::vector<double> > > images;

        /****** Private member functions ******/
        // Recalculate all model images
		void calculate_images(bool update);

        // Recalculate one model image
        void calculate_image(int img, bool update);

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

