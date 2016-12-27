#include "Data.h"
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

Data Data::instance;

Data::Data()
{

}

void Data::load(const char* metadata_file, const char* image_file,
			const char* sigma_file)
{
	/*
	* First, read in the metadata
	*/
	fstream fin(metadata_file, ios::in);
	if(!fin)
		cerr<<"# ERROR: couldn't open file "<<metadata_file<<"."<<endl;
	fin>>num_images>>ni>>nj;
	fin>>x_min>>x_max>>y_min>>y_max;
	fin.close();

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
	fin.open(sigma_file, ios::in);
	if(!fin)
		cerr<<"# ERROR: couldn't open file "<<sigma_file<<"."<<endl;
	sigmas.assign(num_images, vector< vector<double> >(ni, vector<double>(nj)));
	for(int img=0; img<num_images; img++)
		for(int i=0; i<ni; i++)
			for(int j=0; j<nj; j++)
				fin>>sigmas[img][i][j];
	fin.close();

    // Now save the filenames used to run_data.txt
    fstream fout("run_data.txt", ios::out);
    fout<<metadata_file<<endl;
    fout<<image_file<<endl;
    fout<<sigma_file<<endl;
    fout.close();
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

