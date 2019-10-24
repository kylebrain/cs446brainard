#include "Simulation.h"
#include "Utils.h"

Simulation::Simulation(ConfigFile & _configFile, MetaData & _metaData, Logger & _logger):
    configFile(_configFile),
    metaData(_metaData),
    logger(_logger),
    current_pid(0),
    currentMemoryBlock(0)
{
    start_time = std::chrono::system_clock::now();
    createProcesses();
    createIoLocks();
}

void Simulation::run()
{
    logger.log(std::cout) << std::to_string(Utils::s_since(start_time)) << " - Simulator program starting" << std::endl;
    for(Process process : processes)
    {
        logger.log(std::cout) << std::to_string(Utils::s_since(start_time)) << " - OS: preparing process " << process.pid << std::endl;
        logger.log(std::cout) << std::to_string(Utils::s_since(start_time)) << " - OS: starting process " << process.pid << std::endl;
        process.run();
        logger.log(std::cout) << std::to_string(Utils::s_since(start_time)) << " - OS: removing process " << process.pid << std::endl;
    }
    logger.log(std::cout) << std::to_string(Utils::s_since(start_time)) << " - Simulator program ending" << std::endl;
}

void Simulation::createProcesses()
{
    Process * current_process = NULL;
    for(MetaDataItem item : metaData.metaDataItems)
    {
        if(item.code == OPERATING_SYSTEM)
        {
            continue;
        }

        if(item.code == PROGRAM_APPLICATION)
        {
            if(item.descriptor == BEGIN)
            {
                if(current_process != NULL)
                {
                    throw SimError("Current process in progress when A{begin} was encountered");
                }
                current_process = new Process(++current_pid, logger, *this, start_time);
            } else if(current_process != NULL)
            {
                processes.push_back(*current_process);
                delete current_process;
                current_process = NULL;
            } else
            {
                throw SimError("Current process did not exist when A{finish} was encountered");
            }
        } else if(current_process != NULL)
        {
           current_process->operations.push_back(Operation(item, metaData));
        } else
        {
            throw SimError("Encountered code \'" + string(item.code, 1) + "\' when there was no current process");
        }
    }
}

void Simulation::createIoLocks()
{
    for (std::map<string, int>::iterator it = configFile.resourceCount.begin(); it != configFile.resourceCount.end(); it++ )
    {
        std::pair<string, sem_t> lock;
        lock.first = it->first;
        sem_init(&lock.second, 0, it->second);
        ioLocks.insert(lock);

        std::pair<string, int> num;
        num.first = it->first;
        num.second = 0;
        resourceNum.insert(num);
    }
}