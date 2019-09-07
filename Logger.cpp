#include "Logger.h"

Logger::Logger() :
    logType(BOTH),
    logFileName("")
    {}

Logger::Logger(string _logFileName, LogType _logType):
    logType(_logType),
    logFileName(_logFileName)
{
    if(logType == BOTH || logType == LOGFILE)
    {
        logFile.open(logFileName);
    }
}

Logger::~Logger()
{
    if(logFile)
    {
        logFile.close();
    }
}

Logger & operator << (Logger & logger, std::ostream& (*pf)(std::ostream&))
{
    switch(logger.logType)
    {
        case BOTH:
            if(!logger.logFile)
            {
                throw std::runtime_error("Logging is set to both, but the log file is not open");
            }
            logger.logFile << pf;
            std::cout << pf;
            break;
        case LOG_MONITOR:
            std::cout << pf;
            break;
        case LOGFILE:
            if(!logger.logFile)
            {
                throw std::runtime_error("Logging is set to log file, but the log file is not open");
            }
            logger.logFile << pf;
            break;
        default:
            throw std::runtime_error("Logtype \"" + std::to_string(logger.logType) + "\" is not handled in the logger operator <<");
    }

    return logger;
}