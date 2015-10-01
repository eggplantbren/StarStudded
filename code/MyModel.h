#ifndef _MyModel_
#define _MyModel_

#include "Model.h"
#include <vector>
#include <RJObject.h>
#include "MyDistribution.h"

class MyModel:public DNest3::Model
{
	private:
		RJObject<MyDistribution> objects;

		// The model image
		std::vector< std::vector<long double> > image;
		void calculate_image();

		// How many steps since image was computed from scratch
		int staleness;

		// Noise standard deviation
		double sigma;

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

