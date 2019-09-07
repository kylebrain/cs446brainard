#include "Utils.h"

#include <string>
#include <exception>

void Utils::RemoveHeader(string header, std::ifstream & file)
{
    string line;
    std::getline(file, line);
    if(line != header)
    {
        throw std::runtime_error("Config file header did not match: \"" + header + "\"");
    }
}

int Utils::stoiNoSpace(string str)
{
    size_t end_stoi_index;
    int ret = std::stoi(str, &end_stoi_index);

    if(end_stoi_index != str.size())
    {
        throw std::invalid_argument("String \"" + str + "\" to be converted to int contains spaces");
    }

    return ret;
}