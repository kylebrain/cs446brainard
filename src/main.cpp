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
        std::cout << "Creating ConfigFile..." << std::endl;
        ConfigFile configFile(argv[1]);
        std::cout << "Creating Logger..." << std::endl;
        Logger logger(configFile.logFilePath, configFile.logType);

        try
        {
            Utils::VerifyFileExtension(configFile.filePath, META_DATA_FILE_EXTENSION);
            std::cout << "Creating Metadata..." << std::endl;
            MetaData metaData(configFile.filePath);
            std::cout << "FINISHED SETUP" << std::endl;
            std::cout << "--------------" << std::endl << std::endl;
            logger.log(std::cout) << configFile << std::endl;
            logger.log(std::cout) << std::endl;
            metaData.printCycleTimes(configFile, logger);
        } catch (const std::exception & e)
        {
            logger.log(std::cerr) << e.what() << std::endl;
            logger.log(std::cerr) << "Simulation failed" << std::endl;
            return -1;
        }
        
    } catch (const std::exception & e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << "Simulation failed" << std::endl;
        return -1;
    }

    return 0;
}