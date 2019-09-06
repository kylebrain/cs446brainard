#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <fstream>
#include <exception>

using std::string;

class Utils
{
    public:
        static void RemoveHeader(string header, std::ifstream & file);
        static int stoiNoSpace(string str);
};

#endif