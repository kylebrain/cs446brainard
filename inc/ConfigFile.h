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

class ConfigFile
{
    public:
        ConfigFile();
        ConfigFile(string fileName);
        friend std::ostream & operator << (std::ostream &out, const ConfigFile &c);
        string version;
        string filePath;
        int processorCycleTime;
        int keyBoardCycleTime;
        int monitorDisplayTime;
        int hardDriveCycleTime;
        int mouseCycleTime;
        int printerCycleTime;
        int memoryCycleTime;
        LogType logType;
        string logFilePath;

    private:
        void parseConfileFile(string fileName);
        string GetConfigAttribute(string attrHeader, std::ifstream & file);
        LogType GetLogTypeFromString(string logTypeStr);

};

std::ostream & operator << (std::ostream &out, const ConfigFile &c);


#endif