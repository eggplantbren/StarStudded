#include "MultiBandData.h"

MultiBandData::MultiBandData()
{

}

void MultiBandData::load(const char* metadata_file, const char* image_file, const char* sigma_file)
{
	images.push_back(Data());
	images.back().load(metadata_file, image_file, sigma_file);
}

