#ifndef PROCESS_H_
#define PROCESS_H_

#include <vector>
#include <chrono>
#include <map>

#include "Logger.h"
#include "PCB.h"
#include "Operation.h"

class Simulation;

using std::string;

const std::map<string, string> ioShortnames {
    {HARD_DRIVE, "HDD"},
    {KEYBOARD, "KBD"},
    {MOUSE, "MOU"},
    {MONITOR, "MON"},
    {PRINTER, "PRIN"}
};

class Process
{
    public:
        int pid;
        Logger & logger;
        Simulation & sim;
        PCB pcb;
        std::vector<Operation> operations;
        Process(int _pid, Logger & _logger, Simulation & _sim, std::chrono::time_point<std::chrono::system_clock> _sim_start_time);
        void run();

    private:
        std::chrono::time_point<std::chrono::system_clock> sim_start_time;
        string GetOutputString(Operation op, bool start);
        void PerformOperation(Operation op);
        string HexMemoryLocation();
};

#endif