#include "Utils.h"

#include <string>
#include <exception>

// Verifies the next line of a file matches the passed header
void Utils::RemoveHeader(string header, std::ifstream & file)
{
    string line;
    std::getline(file, line);
    if(line != header)
    {
        throw SimError("Config file header did not match: \"" + header + "\". Recieved \"" + line + "\".");
    }
}

// Normally stoi allows for spaces in the string and returns the int preceeding the space
// This is invalid behavoir, so this function verifies that the string only contains an int
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

// Verifies the the fileName ends witht the passed extension
void Utils::VerifyFileExtension(string fileName, string extension)
{
    if(fileName.substr(fileName.find_last_of(".") + 1) != extension)
    {
        throw SimError("Config file extension of \"" + fileName + "\" does not have extension \"." + extension + "\"");
    }
}