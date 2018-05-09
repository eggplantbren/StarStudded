#ifndef StarStudded_Data_h
#define StarStudded_Data_h

#include <vector>

class Data
{
	private:
		// Number of images
		int num_images;

		// Number of pixels
		int ni, nj;

		// Coordinates of image boundaries
		double x_min, x_max, y_min, y_max;

		// Pixel widths
		double dx, dy;

		// Coordinates of pixel centers
		std::vector< std::vector<double> > x_rays;
		std::vector< std::vector<double> > y_rays;

		// The pixels
		std::vector< std::vector< std::vector<double> > > images;

		// Sigma map
		std::vector< std::vector< std::vector<double> > > sigmas;

		void compute_ray_grid();

	public:
		Data();
		void load(const char* metadata_file, const char* image_file,
				const char* sigma_file);

		// Getters
		int get_num_images() const { return num_images; }
		int get_ni() const { return ni; }
		int get_nj() const { return nj; }
		double get_x_min() const { return x_min; }
		double get_x_max() const { return x_max; }
		double get_y_min() const { return y_min; }
		double get_y_max() const { return y_max; }
		double get_x_range() const { return x_max - x_min; }
		double get_y_range() const { return y_max - y_min; }
		double get_dx() const { return dx; }
		double get_dy() const { return dy; }
		const std::vector< std::vector<double> >& get_x_rays() const
			{ return x_rays; }
		const std::vector< std::vector<double> >& get_y_rays() const
			{ return y_rays; }
		const std::vector< std::vector< std::vector<double> > >& get_images() const
			{ return images; }
		const std::vector< std::vector< std::vector<double> > >& get_sigmas() const
			{ return sigmas; }

	// Singleton
	private:
		static Data instance;
	public:
		static Data& get_instance() { return instance; }
};

#endif

