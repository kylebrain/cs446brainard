#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <fstream>
#include <exception>

using std::string;

class SimError : public std::runtime_error
{
    public:
        SimError(const string & msg) : std::runtime_error(msg) {}
};

class Utils
{
    public:
        static void RemoveHeader(string header, std::ifstream & file);
        static int stoiNoSpace(string str);
        static void VerifyFileExtension(string fileName, string extension);
};

#endif