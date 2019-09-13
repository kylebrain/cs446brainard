#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <fstream>
#include <exception>

using std::string;

// Allows for error handling within the simulation
class SimError : public std::runtime_error
{
    public:
        SimError(const string & msg) : std::runtime_error(msg) {}
};

// Defines functions to be used throughout the code
class Utils
{
    public:
        static void RemoveHeader(string header, std::ifstream & file);
        static int stoiNoSpace(string str);
        static void VerifyFileExtension(string fileName, string extension);
};

#endif