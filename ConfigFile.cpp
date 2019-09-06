#include "ConfigFile.h"
#include "Utils.h"
#include <string.h>

ConfigFile::ConfigFile() :
    version(""),
    filePath(""),
    projectorCycleTime(0),
    processorCycleTime(0),
    keyBoardCycleTime(0),
    monitorDisplayTime(0),
    scannerCycleTime(0),
    hardDriveCycleTime(0),
    logType(BOTH),
    logFilePath("")
    {}

ConfigFile::ConfigFile(string fileName) : ConfigFile()
{
    parseConfileFile(fileName);
}

void ConfigFile::parseConfileFile(string fileName)
{
    std::ifstream configFile(fileName);
    if (!configFile)
    {
        throw std::invalid_argument("Could not open specified config file name \"" + fileName +"\"! Was it mispelled?");
    }

    Utils::RemoveHeader("Start Simulator Configuration File", configFile);
    version = GetConfigAttribute("Version/Phase", configFile);
    filePath = GetConfigAttribute("File Path", configFile);
    projectorCycleTime = std::stoi(GetConfigAttribute("Projector cycle time {msec}", configFile));
    processorCycleTime = std::stoi(GetConfigAttribute("Processor cycle time {msec}", configFile));
    keyBoardCycleTime = std::stoi(GetConfigAttribute("Keyboard cycle time {msec}", configFile));
    monitorDisplayTime = std::stoi(GetConfigAttribute("Monitor display time {msec}", configFile));
    scannerCycleTime = std::stoi(GetConfigAttribute("Scanner cycle time {msec}", configFile));
    hardDriveCycleTime = std::stoi(GetConfigAttribute("Hard drive cycle time {msec}", configFile));
    logType = GetLogTypeFromString(GetConfigAttribute("Log", configFile));
    logFilePath = GetConfigAttribute("Log File Path", configFile);
    Utils::RemoveHeader("End Simulator Configuration File", configFile);

    if(!configFile.eof())
    {
        throw std::runtime_error("Config file was successfully processed, but there is extra information");
    }

    configFile.close();


}

string ConfigFile::GetConfigAttribute(string attrHeader, std::ifstream & file)
{
    string line;
    std::getline(file, line);
    char cStringRet[50];
    std::string format = attrHeader + ": %[^\n]";
    if (sscanf(line.c_str(), format.c_str(), cStringRet) != 1)
    {
        throw std::runtime_error("Field \"" + attrHeader + "\" could not be found");
    }
    string retValue(cStringRet);
    return retValue;

}

LogType ConfigFile::GetLogTypeFromString(string logTypeStr)
{
    LogType logType;
    try
    {
        logType =  LOG_TYPE_STRING_MAP.at(logTypeStr);
    }
    catch (std::exception e)
    {
        throw std::runtime_error("Log Type string \"" + logTypeStr + "\" is invalid, does it match with the string dict?");
    }
}

std::ostream & operator << (std::ostream &out, const ConfigFile &c)
{
    out << "Version: " << c.version << std::endl;
    out << "File Path: " << c.filePath << std::endl;
    out << "Projector: " << c.projectorCycleTime << std::endl;
    out << "Processor: " << c.processorCycleTime << std::endl;
    out << "Keyboard: " << c.keyBoardCycleTime << std::endl;
    out << "Monitor: " << c.monitorDisplayTime << std::endl;
    out << "Scanner: " << c.scannerCycleTime << std::endl;
    out << "Hard drive: " << c.hardDriveCycleTime << std::endl;
    out << "Log: " << c.logType << std::endl;
    out << "Log File Path: " << c.logFilePath;
    return out;
}

