#include "Data.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include "yaml-cpp/yaml.h"

using namespace std;

namespace StarStudded
{

Data Data::instance;

Data::Data()
{

}

void Data::load(const char* setup_file)
{
    // Load the run information from the YAML file
    YAML::Node config;
    try
    {
        config = YAML::LoadFile(setup_file);
    }
    catch(...)
    {
        std::cerr << "# Couldn't open or parse ";
        std::cerr << setup_file << '.' << std::endl;
        std::cerr << "# Aborting." << std::endl;
        return;
    }

	/*
	* First, read in the metadata
	*/
    std::string metadata_file = config["data_files"]["metadata_file"]
                                        .as<std::string>();
    YAML::Node metadata;
	try
    {
        metadata = YAML::LoadFile(metadata_file);
    }
    catch(...)
    {
        std::cerr << "# Couldn't open or parse ";
        std::cerr << setup_file << '.' << std::endl;
        std::cerr << "# Aborting." << std::endl;
        return;
    }
    num_images = metadata["num_images"].as<int>();
    ni = metadata["ni"].as<int>();
    nj = metadata["nj"].as<int>();
    x_min = metadata["x_min"].as<double>();
    x_max = metadata["x_max"].as<double>();
    y_min = metadata["y_min"].as<double>();
    y_max = metadata["y_max"].as<double>();

    unsigned int num_pixels = ni*nj*num_images;
    double log10_num_pixels = log10(num_pixels);

    if(log10_num_pixels > 6.0)
    {
        std::cout << "# WARNING: Lots of pixels. You should reconsider life ";
        std::cout << "choices, and/or take a subset of your image.";
        std::cerr << std::endl;
    }

	// Make sure maximum > minimum
	if(x_max <= x_min || y_max <= y_min)
		cerr<<"# ERROR: strange input in "<<metadata_file<<"."<<endl;

	// Compute pixel widths
	dx = (x_max - x_min)/nj;
	dy = (y_max - y_min)/ni;

	// Check that pixels are square
	if(abs(log(dx/dy)) >= 1E-3)
		cerr<<"# ERROR: pixels aren't square."<<endl;

	compute_ray_grid();

	/*
	* Now, load the image
	*/
    std::string image_file = config["data_files"]["images_file"]
                                        .as<std::string>();
    std::fstream fin;
	fin.open(image_file, ios::in);
	if(!fin)
		cerr<<"# ERROR: couldn't open file "<<image_file<<"."<<endl;
	images.assign(num_images, vector< vector<double> >(ni, vector<double>(nj)));
	for(int img=0; img<num_images; img++)
		for(int i=0; i<ni; i++)
			for(int j=0; j<nj; j++)
				fin>>images[img][i][j];
	fin.close();

	/*
	* Load the sigma map
	*/
    std::string sigma_file = config["data_files"]["sigmas_file"]
                                        .as<std::string>();
	fin.open(sigma_file, ios::in);
	if(!fin)
		cerr<<"# ERROR: couldn't open file "<<sigma_file<<"."<<endl;
	sigmas.assign(num_images, vector< vector<double> >(ni, vector<double>(nj)));
	for(int img=0; img<num_images; img++)
		for(int i=0; i<ni; i++)
			for(int j=0; j<nj; j++)
				fin>>sigmas[img][i][j];
	fin.close();
}

void Data::compute_ray_grid()
{
	// Make vectors of the correct size
	x_rays.assign(ni, vector<double>(nj));
	y_rays.assign(ni, vector<double>(nj));

	// Distance between adjacent rays
	double L = dx;

	for(size_t i=0; i<x_rays.size(); i++)
	{
		for(size_t j=0; j<x_rays[i].size(); j++)
		{
			x_rays[i][j] = x_min + (j + 0.5)*L;
			y_rays[i][j] = y_max - (i + 0.5)*L;
		}
	}
}

} // namespace StarStudded

