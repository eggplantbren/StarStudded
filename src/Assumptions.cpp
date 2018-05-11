#include "Assumptions.h"
#include <iostream>
#include "yaml-cpp/yaml.h"

namespace StarStudded
{

// Construct the static instance
Assumptions Assumptions::instance;

void Assumptions::load(const char* setup_filename)
{
    // Load stuff from the YAML file
    YAML::Node config;
    try
    {
        config = YAML::LoadFile(setup_filename);
    }
    catch(...)
    {
        std::cerr << "# Couldn't open or parse ";
        std::cerr << setup_filename << '.' << std::endl;
        std::cerr << "# Aborting." << std::endl;
        return;
    }

    max_num_stars = config["assumptions"]["max_num_stars"].as<unsigned int>();
    padding = config["assumptions"]["padding"].as<double>();

    if(max_num_stars > 3000)
    {
        std::cerr << "# WARNING: You're trying to find a very large number ";
        std::cerr << "of stars. This could take a while." << std::endl;
    }
}


} // namespace StarStudded

