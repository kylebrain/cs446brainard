#include "Logger.h"
#include "Utils.h"

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

StreamLogger Logger::log(std::ostream & stream)
{
    return StreamLogger(stream, *this);
}

StreamLogger::StreamLogger(const StreamLogger & sLogger):
    outputStream(sLogger.outputStream),
    logger(sLogger.logger)
{}

StreamLogger::StreamLogger(std::ostream & _outputStream, Logger & _logger):
    outputStream(_outputStream),
    logger(_logger)
{}

StreamLogger operator << (StreamLogger streamLogger, std::ostream& (*pf)(std::ostream&))
{
    switch(streamLogger.logger.logType)
    {
        case BOTH:
            if(!streamLogger.logger.logFile)
            {
                throw SimError("Logging is set to both, but the log file is not open");
            }
            streamLogger.logger.logFile << pf;
            streamLogger.outputStream << pf;
            break;
        case LOG_MONITOR:
            streamLogger.outputStream << pf;
            break;
        case LOGFILE:
            if(!streamLogger.logger.logFile)
            {
                throw SimError("Logging is set to log file, but the log file is not open");
            }
            streamLogger.logger.logFile << pf;
            break;
        default:
            throw SimError("Logtype \"" + std::to_string(streamLogger.logger.logType) + "\" is not handled in the logger operator <<");
    }

    return streamLogger;
}