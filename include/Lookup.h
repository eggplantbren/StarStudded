#ifndef StarStudded_Lookup_h
#define StarStudded_Lookup_h

#include <vector>

namespace StarStudded
{

/*
* Lookup tables for speeding things up
* Singleton pattern
*/

class Lookup
{
	private:
		int num;
		double xMin, xMax, dx, one_over_dx;
		std::vector<double> _exp; // exp(-x) for x >= 0

		Lookup();
		Lookup(const Lookup& other);

		static Lookup instance;

	public:
		static double evaluate(double x);

};

} // namespace StarStudded

#endif

