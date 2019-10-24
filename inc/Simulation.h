#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <vector>
#include <map>
#include <chrono>
#include <semaphore.h>

#include "Simulation.h"

#include "ConfigFile.h"
#include "MetaData.h"
#include "Logger.h"
#include "Process.h"

using std::string;

class Simulation
{
    public:
        ConfigFile & configFile;
        MetaData & metaData;
        Logger & logger;

        std::vector<Process> processes;
        std::map<string, sem_t> ioLocks;
        std::map<string, int> resourceNum;
        long currentMemoryBlock;

        Simulation(ConfigFile & _configFile, MetaData & _metaData, Logger & _logger);
        void run();

    private:
        void createProcesses();
        void createIoLocks();
        int current_pid;
        std::chrono::time_point<std::chrono::system_clock> start_time;
};


#endif