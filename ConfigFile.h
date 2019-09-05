#ifndef CONFIG_FILE_H_
#define CONFIG_FILE_H_

#include <string>
#include <fstream>
#include <exception>
#include <iostream>
#include <map>

using std::string;

enum LogType {BOTH, MONITOR, LOGFILE};
const string LOG_TYPE_HEADER = "Log to ";
const std::map<string, LogType> LOG_TYPE_STRING_MAP =
{
    {LOG_TYPE_HEADER + "Both", BOTH},
    {LOG_TYPE_HEADER + "Monitor", MONITOR},
    {LOG_TYPE_HEADER + "Log file", LOGFILE}
};

class ConfigFile
{
    public:
        ConfigFile();
        ConfigFile(string fileName);
        friend std::ostream & operator << (std::ostream &out, const ConfigFile &c);
        string version;
        string filePath;
        int projectorCycleTime;
        int processorCycleTime;
        int keyBoardCycleTime;
        int monitorDisplayTime;
        int scannerCycleTime;
        int hardDriveCycleTime;
        LogType logType;
        string logFilePath;

    private:
        void parseConfileFile(string fileName);
        string GetConfigAttribute(string attrHeader, std::ifstream & file);
        void RemoveHeader(string header, std::ifstream & file);
        LogType GetLogTypeFromString(string logTypeStr);

};

std::ostream & operator << (std::ostream &out, const ConfigFile &c);


#endif