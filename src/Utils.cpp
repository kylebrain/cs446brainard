#include "Utils.h"

#include <string>
#include <exception>

void Utils::RemoveHeader(string header, std::ifstream & file)
{
    string line;
    std::getline(file, line);
    if(line != header)
    {
        throw SimError("Config file header did not match: \"" + header + "\". Recieved \"" + line + "\".");
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

void Utils::VerifyFileExtension(string fileName, string extension)
{
    if(fileName.substr(fileName.find_last_of(".") + 1) != extension)
    {
        throw SimError("Config file extension of \"" + fileName + "\" does not have extension \"." + extension + "\"");
    }
}