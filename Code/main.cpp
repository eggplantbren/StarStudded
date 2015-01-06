#include <iostream>
#include "Start.h"
#include "MyModel.h"
#include "Data.h"

using namespace std;
using namespace DNest3;

int main(int argc, char** argv)
{
	Data::get_instance().load("Data/test_metadata.txt",
				"Data/test_image.txt", "Data/test_sigma.txt");

	MTSampler<MyModel> sampler = setup_mt<MyModel>(argc, argv);
	sampler.run();

	return 0;
}

