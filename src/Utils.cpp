#include "Utils.h"

#include <string>
#include <exception>
#include <iostream>
#include <pthread.h>

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

void Utils::wait(int durations_ms)
{
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    float timePassed = 0;
    do
    {
        timePassed = ms_since(start);
    } while (timePassed < durations_ms);
}

void * Utils::wait_void(void * duration_ms)
{
    int * arg = (int *) duration_ms;
    wait(*arg);
    pthread_exit(NULL);
}

void Utils::threaded_wait(int duration_ms)
{
    pthread_t thread;
    pthread_attr_t attr;
    void * status;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    int rc = pthread_create(&thread, &attr, Utils::wait_void, (void *) &duration_ms);
    if(rc)
    {
        std::cerr << "Unable to create thread, " << rc << std::endl;
    }

    pthread_attr_destroy(&attr);

    rc = pthread_join(thread, &status);
    if(rc)
    {
        std::cerr << "Unable to join thread, " << rc << std::endl;
    }
}

float Utils::ms_since(std::chrono::time_point<std::chrono::system_clock> start)
{
    std::chrono::time_point<std::chrono::system_clock> current = std::chrono::system_clock::now();
    std::chrono::milliseconds milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(current - start);
    return milliseconds.count();
}

float Utils::s_since(std::chrono::time_point<std::chrono::system_clock> start)
{
    std::chrono::time_point<std::chrono::system_clock> current = std::chrono::system_clock::now();
    std::chrono::duration<float> seconds = current - start;
    return seconds.count();
}