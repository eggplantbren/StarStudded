#include <iostream>
#include "DNest4/code/Start.h"
#include "Assumptions.h"
#include "Data.h"
#include "MyModel.h"

using namespace DNest4;
using namespace StarStudded;

int main(int argc, char** argv)
{
	Data::get_instance().load("setup.yaml");
    Assumptions::get_instance().load("setup.yaml");

	RNG::randh_is_randh2 = true;
	Sampler<MyModel> sampler = setup<MyModel>(argc, argv);
	sampler.run();

	return 0;
}

