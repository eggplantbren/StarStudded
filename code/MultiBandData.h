#ifndef _MultiBandData_
#define _MultiBandData_

#include "Data.h"
#include <vector>

class MultiBandData
{
	private:
		std::vector<Data> images;

	public:
		MultiBandData();
		void load(const char* metadata_file, const char* image_file, const char* sigma_file);

};

#endif

