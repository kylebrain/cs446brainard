#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <vector>

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

        Simulation(ConfigFile _configFile, MetaData _metaData, Logger _logger);
        void run();
        void createProcesses();
};


#endif