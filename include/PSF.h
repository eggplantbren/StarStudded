#ifndef StarStudded_PSF_h
#define StarStudded_PSF_h

#include <ostream>
#include "DNest4/code/RNG.h"

namespace StarStudded
{

// Pixel-convolved PSF
class PSF
{
	public:
		static const double edge; // How many sigmas out to set PSF to zero

	private:
		// Fundamental parameters
		double sigma1, sigma2, weight, q, theta;

		// Derived parameters
		double pr1, pr2; // pr = 1/sigma^2
		double preFactor1, preFactor2;
		double rEdge, rsqEdge;
		double cosTheta, sinTheta;

		// Keep derived parameters in sync with fundamental ones
		void sync();

	public:
		PSF();
		PSF(double sigma1, double sigma2, double weight, double q,
			double theta);
		double evaluate(double x, double y) const;
		double get_rEdge() const { return rEdge; }

		// Can infer PSF!
		void from_prior(DNest4::RNG& rng);
		double perturb(DNest4::RNG& rng);

		// Getters
		double get_sigma2() const { return sigma2; }

		friend std::ostream& operator << (std::ostream& out,
							const PSF& psf);
};

} // namespace StarStudded

#endif

