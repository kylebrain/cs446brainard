#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <fstream>
#include <iostream>

using std::string;

enum LogType {BOTH, LOG_MONITOR, LOGFILE};

class Logger
{
    public:
        Logger();
        Logger(string _logFileName, LogType _logType);
        ~Logger();

        template<class T>
        friend Logger & operator<<(Logger & logger, const T & data);
        friend Logger & operator<<(Logger & logger, std::ostream& (*pf)(std::ostream&));

        LogType logType;
        string logFileName;

    private:
        std::ofstream logFile;
};

template<class T>
Logger & operator << (Logger & logger, const T & data)
{
    switch(logger.logType)
    {
        case BOTH:
            if(!logger.logFile)
            {
                throw std::runtime_error("Logging is set to both, but the log file is not open");
            }
            logger.logFile << data;
            std::cout << data;
            break;
        case LOG_MONITOR:
            std::cout << data;
            break;
        case LOGFILE:
            if(!logger.logFile)
            {
                throw std::runtime_error("Logging is set to log file, but the log file is not open");
            }
            logger.logFile << data;
            break;
        default:
            throw std::runtime_error("Logtype \"" + std::to_string(logger.logType) + "\" is not handled in the logger operator <<");
    }

    return logger;
}

#endif