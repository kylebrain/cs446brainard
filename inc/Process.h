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
        Logger & logger;
        int totalSystemMemory;
        PCB pcb;
        std::vector<Operation> operations;
        void run();

    private:
        string GetOutputString(Operation op);
        void PerformOperation(Operation op);
};

#endif