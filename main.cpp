#include <iostream>

#include "ConfigFile.h"

int main(void)
{
    ConfigFile configFile("config.txt");
    std::cout << configFile << std::endl;
    return 0;
}