#ifndef PROCESS_H_
#define PROCESS_H_

#include <vector>
#include <chrono>

#include "Logger.h"
#include "PCB.h"
#include "Operation.h"

using std::string;

class Process
{
    public:
        int pid;
        Logger & logger;
        int totalSystemMemory;
        PCB pcb;
        std::vector<Operation> operations;
        Process(int _pid, Logger & _logger, int _totalSystemMemory, std::chrono::time_point<std::chrono::system_clock> _sim_start_time);
        void run();

    private:
        std::chrono::time_point<std::chrono::system_clock> sim_start_time;
        string GetOutputString(Operation op, bool start);
        void PerformOperation(Operation op);
        string RandomHexMemoryLocation();
};

#endif