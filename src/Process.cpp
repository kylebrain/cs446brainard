#include "Process.h"
#include "MetaData.h"
#include "Utils.h"

#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>

Process::Process(int _pid, Logger & _logger, int _totalSystemMemory, std::chrono::time_point<std::chrono::system_clock> _sim_start_time):
    pid(_pid),
    logger(_logger),
    totalSystemMemory(_totalSystemMemory),
    sim_start_time(_sim_start_time)
{
    std::srand(std::time(NULL));

    pcb.processState = START;
    pcb.processState = READY;
}

void Process::run()
{
    pcb.processState = RUNNING;
    for(Operation op : operations)
    {
        logger.log(std::cout) << GetOutputString(op, true) << std::endl;
        PerformOperation(op);
        logger.log(std::cout) << GetOutputString(op, false) << std::endl;
    }
    pcb.processState = EXIT;
}

string Process::GetOutputString(Operation op, bool start)
{
    string ret;

    ret += std::to_string(Utils::s_since(sim_start_time)) + " - ";
    ret += "Process " + std::to_string(pid) + ": "; 

    if(op.item.descriptor != ALLOCATE)
    {
        ret += start ? "start" : "end";
        ret += " "; 

        if(op.item.code == PROCESS)
        {
            ret += "processing action";
        } else if(op.item.descriptor == BLOCK)
        {
            ret += "memory blocking";
        } else
        {
            ret += op.item.descriptor;
            if(op.item.code == OUTPUT)
            {
                ret += " output";
            } else if(op.item.code == INPUT)
            {
                ret += " input";
            } else
            {
                ret += " NOT HANDLED CORRECTLY!";
            }
        }
    } else
    {
        ret += start ? "allocating memory" : ("memory allocated at " + RandomHexMemoryLocation());
    }
    return ret;
}

void Process::PerformOperation(Operation op)
{
    if(op.item.code == OUTPUT || op.item.code == INPUT)
    {
        pcb.processState = WAIT;
        Utils::threaded_wait(op.cycleTime);
        pcb.processState = RUNNING;
    } else
    {
        Utils::wait(op.cycleTime);
    }
}

string Process::RandomHexMemoryLocation()
{
    int memoryAddress = std::rand() % (totalSystemMemory * 8000);
    std::stringstream stream;
    stream << "0x" << std::setfill('0') << std::setw(8) << std::hex << memoryAddress;
    return stream.str();
}
