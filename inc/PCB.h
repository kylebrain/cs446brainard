#ifndef PCB_H_
# define PCB_H_

enum ProcessState {START, READY, RUNNING, WAIT, EXIT};

class PCB
{
    public:
        ProcessState processState;
};

#endif