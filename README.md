# CS446 Project 1
## Description
This project reads a config file for operation cycle times and then reads a metadata file for the performed operations. It then outputs the cycle times for each operation.

## Implementation
The config file parser using the sscanf function to scan each line for the required field and gets the value from it.\
The metadata parser uses a state machine scanner to read each character and transition into reading each field based on the read character.\
Logging is performed with the Logger class which ultilizes a StreamLogger class to be able to log with the << operator.

## Running
```bash
make
./sim.exe $config_file.conf
```
