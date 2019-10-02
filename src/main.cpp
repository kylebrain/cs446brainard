#include <iostream>
#include <exception>
#include <string>

#include "ConfigFile.h"
#include "MetaData.h"
#include "Logger.h"
#include "Utils.h"

int main(int argc, char * argv[])
{
    if(argc < 2)
    {
        std::cerr << "Please enter the config file name!" << std::endl;
        return -1;
    }

    try
    {
        Utils::VerifyFileExtension(argv[1], CONF_FILE_EXTENSION);
        ConfigFile configFile(argv[1]);
        Logger logger(configFile.logFilePath, configFile.logType);
        logger.log(std::cout) << configFile << std::endl;
        logger.log(std::cout) << std::endl;

        try
        {
            Utils::VerifyFileExtension(configFile.filePath, META_DATA_FILE_EXTENSION);
            MetaData metaData(logger, configFile);
            logger.log(std::cout) << std::endl;

            
        } catch (const SimError & e)
        {
            logger.log(std::cerr) << e.what() << std::endl;
            logger.log(std::cerr) << "Simulation failed" << std::endl;
            return -1;
        }
        
    } catch (const SimError & e)
    {
        // Output any errors to std::cerr before the logger is built
        std::cerr << e.what() << std::endl;
        std::cerr << "Simulation failed" << std::endl;
        return -1;
    }

    return 0;
}