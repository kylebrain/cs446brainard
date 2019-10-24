#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <fstream>
#include <exception>
#include <chrono>
#include <semaphore.h>

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
        static void wait(int durations_ms);
        static void * wait_void(void * duration_ms);
        static void threaded_wait(int duration_ms, sem_t * sem);
        static float ms_since(std::chrono::time_point<std::chrono::system_clock> start);
        static float s_since(std::chrono::time_point<std::chrono::system_clock> start);
};

struct io_wait
{
    io_wait(sem_t * _sem, int _ms) : sem(_sem), ms(_ms) {}
    sem_t * sem;
    int ms;
};

#endif