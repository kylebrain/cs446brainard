#include "ConfigFile.h"
#include "Utils.h"
#include <string.h>

ConfigFile::ConfigFile() :
    version(""),
    filePath(""),
    processorCycleTime(0),
    keyBoardCycleTime(0),
    monitorDisplayTime(0),
    hardDriveCycleTime(0),
    mouseCycleTime(0),
    printerCycleTime(0),
    memoryCycleTime(0),
    logType(BOTH),
    logFilePath("")
    {}

ConfigFile::ConfigFile(string fileName) : ConfigFile()
{
    parseConfileFile(fileName);
}

// Parses the given file and fills out the data ConfigFile fields
void ConfigFile::parseConfileFile(string fileName)
{
    std::ifstream configFile(fileName);
    if (!configFile)
    {
        throw SimError("Could not open specified config file name \"" + fileName +"\"! Was it mispelled?");
    }

    if(configFile.peek() == std::ifstream::traits_type::eof())
    {
        throw SimError("Specified metadata file \"" + fileName +"\" is empty");
    }

    Utils::RemoveHeader("Start Simulator Configuration File", configFile);
    version = GetConfigAttribute("Version/Phase", configFile);
    filePath = GetConfigAttribute("File Path", configFile);
    monitorDisplayTime = std::stoi(GetConfigAttribute("Monitor display time {msec}", configFile));
    processorCycleTime = std::stoi(GetConfigAttribute("Processor cycle time {msec}", configFile));
    mouseCycleTime = std::stoi(GetConfigAttribute("Mouse cycle time {msec}", configFile));
    hardDriveCycleTime = std::stoi(GetConfigAttribute("Hard drive cycle time {msec}", configFile));
    keyBoardCycleTime = std::stoi(GetConfigAttribute("Keyboard cycle time {msec}", configFile));
    memoryCycleTime = std::stoi(GetConfigAttribute("Memory cycle time {msec}", configFile));
    printerCycleTime = std::stoi(GetConfigAttribute("Printer cycle time {msec}", configFile));
    logType = GetLogTypeFromString(GetConfigAttribute("Log", configFile));
    logFilePath = GetConfigAttribute("Log File Path", configFile);
    Utils::RemoveHeader("End Simulator Configuration File", configFile);

    configFile.close();

}

// Scans each line for the passed attrHeader and extracts the field's value
string ConfigFile::GetConfigAttribute(string attrHeader, std::ifstream & file)
{
    string line;
    std::getline(file, line);
    char cStringRet[50];
    std::string format = attrHeader + ": %[^\n]";
    if (sscanf(line.c_str(), format.c_str(), cStringRet) != 1)
    {
        throw SimError("Field \"" + attrHeader + "\" could not be found");
    }
    string retValue(cStringRet);
    return retValue;

}

// Looks up the formatted string to get the associated enum value
LogType ConfigFile::GetLogTypeFromString(string logTypeStr)
{
    LogType logType;
    try
    {
        logType =  LOG_TYPE_STRING_MAP.at(logTypeStr);
    }
    catch (std::exception e)
    {
        throw SimError("Log Type string \"" + logTypeStr + "\" is invalid, does it match with the string dict?");
    }
}

// Returns the formatted string for the object's log type
string ConfigFile::logString() const
{
    switch(logType)
    {
        case BOTH:
            return "monitor and " + logFilePath;
        case LOG_MONITOR:
            return "monitor";
        case LOGFILE:
            return logFilePath;
        default:
            break;
    }
}

// Outputs the config file data in a readable format
std::ostream & operator << (std::ostream &out, const ConfigFile &c)
{
    out << "Configuration File Data" << std::endl;

    out << "Monitor = " << c.monitorDisplayTime << " ms/cycle" << std::endl;
    out << "Processor = " << c.processorCycleTime << " ms/cycle" << std::endl;
    out << "Mouse = " << c.mouseCycleTime << " ms/cycle" << std::endl;
    out << "Hard drive = " << c.hardDriveCycleTime << " ms/cycle" << std::endl;
    out << "Keyboard = " << c.keyBoardCycleTime << " ms/cycle" << std::endl;
    out << "Memory = " << c.memoryCycleTime << " ms/cycle" << std::endl;
    out << "Printer = " << c.printerCycleTime << " ms/cycle" << std::endl;
    out << "Logged to: " << c.logString();
    return out;
}

