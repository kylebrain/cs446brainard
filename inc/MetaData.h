#ifndef META_DATA_H_
#define META_DATA_H_

#include <string>
#include <map>
#include <vector>
#include <iostream>

using std::string;

const string META_DATA_FILE_EXTENSION = "mdf";

const char OPERATING_SYSTEM = 'S';
const char PROGRAM_APPLICATION = 'A';
const char PROCESS = 'P';
const char INPUT = 'I';
const char OUTPUT = 'O';
const char MEMORY = 'M';

const string BEGIN = "begin";
const string FINISH = "finish";
const string RUN = "run";
const string HARD_DRIVE = "hard drive";
const string KEYBOARD = "keyboard";
const string MOUSE = "mouse";
const string MONITOR = "monitor";
const string PRINTER = "printer";
const string BLOCK = "block";
const string ALLOCATE = "allocate";

class ConfigFile;
class Logger;

const std::map<char, std::vector<string>> META_DATA_MAP =
{
    {
        OPERATING_SYSTEM,
        {
            BEGIN,
            FINISH
        }
    },
    {
        PROGRAM_APPLICATION,
        {
            BEGIN,
            FINISH
        }
    },
    {
        PROCESS,
        {
            RUN
        }
    },
    {
        INPUT,
        {
            HARD_DRIVE,
            KEYBOARD,
            MOUSE
        }
    },
    {
        OUTPUT,
        {
            HARD_DRIVE,
            MONITOR,
            PRINTER
        }
    },
    {
        MEMORY,
        {
            BLOCK,
            ALLOCATE
        }
    }
};

// Contains data regarding each metadata tag
class MetaDataItem
{
    public:
        MetaDataItem();
        MetaDataItem(char _code, string _descriptor, int _cycle);
        friend std::ostream & operator << (std::ostream &out, const MetaDataItem &mdi);
        string getFormatted();
        void validate();

        char code;
        string descriptor;
        int cycle;
};

std::ostream & operator << (std::ostream &out, const MetaDataItem &mdi);


enum MetaDataParserState {CODE, DESCRIPTOR, CYCLE};

// Contains all relavent data related to the metadata
// Holds a list of metadata tags
class MetaData
{
    public:
        MetaData(Logger & _logger, ConfigFile & configFile);
        std::vector<MetaDataItem> metaDataItems;
        void printCycleTime(MetaDataItem & metaItem);
        const std::map<string, int> cycleMap;

    private:
        Logger & logger;
        MetaDataItem * beginOperatingSystem;
        MetaDataItem * beginApplication;
        MetaDataItem * finishOperatingSystem;
        MetaDataItem * finishApplication;

        void checkForBeginFinish(MetaDataItem & item);
        void parseMetaData(string fileName);
        int getCycleFromString(string cycleString);
        std::vector<MetaDataItem> tokenizeMetaData(std::ifstream & file);

};

#endif