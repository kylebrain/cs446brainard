#include "Process.h"
#include "MetaData.h"
#include "Utils.h"
#include "Simulation.h"

#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>

Process::Process(int _pid, Logger & _logger, Simulation & _sim, std::chrono::time_point<std::chrono::system_clock> _sim_start_time):
    pid(_pid),
    logger(_logger),
    sim(_sim),
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

            if(op.item.descriptor == HARD_DRIVE || op.item.descriptor == PRINTER)
            {
                if(!start)
                {
                    sim.resourceNum[op.item.descriptor]++;
                    sim.resourceNum[op.item.descriptor] %= sim.configFile.resourceCount[op.item.descriptor];
                } else
                {
                    ret += " on " + ioShortnames.at(op.item.descriptor) + " " + std::to_string(sim.resourceNum[op.item.descriptor]);
                }
            }
        }
    } else
    {
        ret += start ? "allocating memory" : ("memory allocated at " + HexMemoryLocation());
    }
    return ret;
}

void Process::PerformOperation(Operation op)
{
    if(op.item.code == OUTPUT || op.item.code == INPUT)
    {
        pcb.processState = WAIT;
        Utils::threaded_wait(op.cycleTime, &sim.ioLocks[op.item.descriptor]);
        pcb.processState = RUNNING;
    } else
    {
        Utils::wait(op.cycleTime);
    }
}

string Process::HexMemoryLocation()
{
    std::stringstream stream;
    stream << "0x" << std::setfill('0') << std::setw(8) << std::hex << sim.currentMemoryBlock;
    sim.currentMemoryBlock += sim.configFile.memoryBlockSize;
    sim.currentMemoryBlock %= sim.configFile.systemMemory;
    return stream.str();
}

int Process::GetIOOperationCount() const
{
    int count = 0;
    for(Operation op : operations)
    {
        if(op.item.code == OUTPUT || op.item.code == INPUT)
        {
            count++;
        }
    }

    return count;
}

bool Process::sortByPid(Process & lhs, Process & rhs)
{
    return lhs.pid > rhs.pid;
}
bool Process::sortByIOOps(Process & lhs, Process & rhs)
{
    return lhs.GetIOOperationCount() < rhs.GetIOOperationCount();
}
bool Process::sortByOps(Process & lhs, Process & rhs)
{
    return lhs.operations.size() > rhs.operations.size();
}

Process& Process::operator=(Process&& rhs)
{
    pid = rhs.pid;
    operations = rhs.operations;
    return *this;
}

Process::Process(const Process& other)
: Process(other.pid, other.logger, other.sim, other.sim_start_time)
{
    operations = other.operations;
}