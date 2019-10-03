# CS446 Project

## Running
```bash
make
./sim2 $config_file.conf
```

## Project 2
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
