#include <iostream>
#include <exception>

#include "ConfigFile.h"

int main(int argc, char * argv[])
{
    if(argc < 2)
    {
        std::cerr << "Please enter the config file name!" << std::endl;
        return -1;
    }

    try
    {
        ConfigFile configFile(argv[1]);
        std::cout << configFile << std::endl;
    } catch (const std::exception & e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << "Simulation failed" << std::endl;
        return -1;
    }

    return 0;
}