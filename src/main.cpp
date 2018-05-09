#include <iostream>
#include "DNest4/code/Start.h"
#include "Assumptions.h"
#include "Data.h"
#include "MyModel.h"

using namespace std;
using namespace DNest4;

int main(int argc, char** argv)
{
	Data::get_instance().load("setup.yaml");
    StarStudded::Assumptions::get_instance().load("setup.yaml");

	Sampler<MyModel> sampler = setup<MyModel>(argc, argv);
	sampler.run();

	return 0;
}

