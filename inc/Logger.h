#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <fstream>
#include <iostream>

using std::string;

enum LogType {BOTH, LOG_MONITOR, LOGFILE};
class StreamLogger;
class SimError;

class Logger
{
    public:
        Logger();
        Logger(string _logFileName, LogType _logType);
        ~Logger();
        StreamLogger log(std::ostream & stream);
        template<class T>
        friend StreamLogger operator<<(StreamLogger streamLogger, const T & data);
        friend StreamLogger operator<<(StreamLogger streamLogger, std::ostream& (*pf)(std::ostream&));

        LogType logType;
        string logFileName;

    private:
        std::ofstream logFile;
};

class StreamLogger
{
    public:
        StreamLogger(const StreamLogger & sLogger);
        StreamLogger(std::ostream & _outputStream, Logger & _logger);
        template<class T>
        friend StreamLogger operator<<(StreamLogger streamLogger, const T & data);
        friend StreamLogger operator<<(StreamLogger streamLogger, std::ostream& (*pf)(std::ostream&));
    private:
        std::ostream & outputStream;
        Logger & logger;
};

template<class T>
StreamLogger operator << (StreamLogger streamLogger, const T & data)
{
    switch(streamLogger.logger.logType)
    {
        case BOTH:
            if(!streamLogger.logger.logFile)
            {
                throw SimError("Logging is set to both, but the log file is not open");
            }
            streamLogger.logger.logFile << data;
            streamLogger.outputStream << data;
            break;
        case LOG_MONITOR:
            streamLogger.outputStream << data;
            break;
        case LOGFILE:
            if(!streamLogger.logger.logFile)
            {
                throw SimError("Logging is set to log file, but the log file is not open");
            }
            streamLogger.logger.logFile << data;
            break;
        default:
            throw SimError("Logtype \"" + std::to_string(streamLogger.logger.logType) + "\" is not handled in the logger operator <<");
    }

    return streamLogger;
}

#endif