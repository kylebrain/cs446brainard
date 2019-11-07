# CS446 Project

## Running
```bash
make
./sim4 $config_file.conf
```

# Project 4
## Relavant Code Locations
Scheduling: The priority queue is set up using different comparisons in Simulation.cpp line 70\
The the comparisons are implements in Process.cpp line 122

## Description
The simulator runs processes in the order that the scheduling algorithm defines.

## Implementation
A priority queue is used to sort the processes based on the scheduling algorithm.

# Project 3
## Relavant Code Locations
Semaphores: defined in Simulation.h line 26 and used in Utils.cpp line 58 and 63

## Description
The simulator must keep track of resources and never assign the same resource to two processes

## Implementation
Semaphores are used to keep track of the resources available and wait and post are used to reserve and release resouces

# Project 2
## Relevant Code Locations
PCB: defined in PCB.h declared in Process.h line 19 and implemented in Process.cpp
Threads: implemented in Utils.cpp line 60

## Description
The simulator runs the operations as defined in the metadata file for the cycle length specified by the config file. A timer is create and ran for each operation.
For I/O operations the timer is run in a seperate thread. A PCB keeps track of the process's status.

## Implementation
The simulator contains a list of processes. Each proccess contains a list of operations.\
The simulator creates the processes by parsing the metadata items.

# Project 1
## Description
This project reads a config file for operation cycle times and then reads a metadata file for the performed operations. It then outputs the cycle times for each operation.

## Implementation
The config file parser using the sscanf function to scan each line for the required field and gets the value from it.\
The metadata parser uses a state machine scanner to read each character and transition into reading each field based on the read character.\
Logging is performed with the Logger class which ultilizes a StreamLogger class to be able to log with the << operator.
