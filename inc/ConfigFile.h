#ifndef CONFIG_FILE_H_
#define CONFIG_FILE_H_

#include <string>
#include <fstream>
#include <exception>
#include <iostream>
#include <map>

#include "Logger.h"

using std::string;

const string CONF_FILE_EXTENSION = "conf";

const string LOG_TYPE_HEADER = "Log to ";
const std::map<string, LogType> LOG_TYPE_STRING_MAP =
{
    {LOG_TYPE_HEADER + "Both", BOTH},
    {LOG_TYPE_HEADER + "Monitor", LOG_MONITOR},
    {LOG_TYPE_HEADER + "File", LOGFILE}
};

enum Scheduler{SCH_FIFO, SCH_PS, SCH_SJF};
const string FIFO = "FIFO";
const string PS = "PS";
const string SJF = "SJF";

const std::map<string, Scheduler> SCHEDULER_STRING_MAP =
{
    {FIFO, SCH_FIFO},
    {PS, SCH_PS},
    {SJF, SCH_SJF}
};

class ConfigFile
{
    public:
        ConfigFile();
        ConfigFile(string fileName);
        friend std::ostream & operator << (std::ostream &out, const ConfigFile &c);
        string version;
        string filePath;
        Scheduler scheduler;
        int quatumNumber;
        int processorCycleTime;
        int keyBoardCycleTime;
        int monitorDisplayTime;
        int hardDriveCycleTime;
        int mouseCycleTime;
        int printerCycleTime;
        int memoryCycleTime;
        int systemMemory;
        int memoryBlockSize;
        LogType logType;
        string logFilePath;
        std::map<string, int> resourceCount;

    private:
        void parseConfileFile(string fileName);
        string GetConfigAttribute(string attrHeader, std::ifstream & file, string * units = NULL);
        LogType GetLogTypeFromString(string logTypeStr);
        Scheduler GetSchedulerFromString(string schedulerStr);
        int GetKiloBytes(string value, string units);
        string logString() const;

};

std::ostream & operator << (std::ostream &out, const ConfigFile &c);


#endif