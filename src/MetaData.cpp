#include "MetaData.h"
#include "Utils.h"
#include "ConfigFile.h"

#include <iostream>
#include <algorithm>
#include <exception>
#include <map>

MetaDataItem::MetaDataItem():
    code('0'),
    descriptor(""),
    cycle(0)
    {}

MetaDataItem::MetaDataItem(char _code, string _descriptor, int _cycle):
    code(_code),
    descriptor(_descriptor),
    cycle(_cycle)
{
    validate();
}

void MetaDataItem::validate()
{
    std::vector<string> descriptorVector;
    try
    {
        descriptorVector = META_DATA_MAP.at(code);
    } catch (std::out_of_range e)
    {
        throw std::runtime_error("Metadata code \"" + string(1, code) + "\" is not valid");
    }

    if(std::find(descriptorVector.begin(), descriptorVector.end(), descriptor) == descriptorVector.end())
    {
        throw std::runtime_error("Metadata Descriptor \"" + descriptor + "\" is not associated with code \"" + string(1, code) + "\"");
    }

    if(cycle < 0)
    {
        throw std::runtime_error("Metadata cycle \"" + std::to_string(cycle) + "\" must be greater or equal to 0");
    }
}

string MetaDataItem::getFormatted()
{
    return string(1, code) + "{" + descriptor + "}" + std::to_string(cycle);
}

std::ostream & operator << (std::ostream &out, const MetaDataItem &mdi)
{
    out << "Code: " << mdi.code << std::endl;
    out << "Descriptor: " << mdi.descriptor << std::endl;
    out << "Cycle number: " << mdi.cycle;
    return out;
}

MetaData::MetaData() {}

MetaData::MetaData(string fileName) : MetaData()
{
    parseMetaData(fileName);
}

void MetaData::parseMetaData(string fileName)
{
    std::ifstream metaDataFile(fileName);
    if (!metaDataFile)
    {
        throw std::invalid_argument("Could not open specified metadata file name \"" + fileName +"\"! Was it mispelled?");
    }

    Utils::RemoveHeader("Start Program Meta-Data Code:", metaDataFile);
    metaDataItems = tokenizeMetaData(metaDataFile);
    Utils::RemoveHeader("End Program Meta-Data Code.", metaDataFile);
    metaDataFile.close();
}

std::vector<MetaDataItem> MetaData::tokenizeMetaData(std::ifstream & file)
{
    std::vector<MetaDataItem> retMetaDataItems;
    MetaDataItem * currentItem = NULL;
    MetaDataParserState state = CODE;
    bool finished = false;
    string cycleString = "";

    try
    {
        while(!finished)
        {
            char currentChar;
            if(state == DESCRIPTOR)
            {
                // "hard drive" must be formatted with a space
                file >> std::noskipws >> currentChar;
            } else
            {
                file >> std::skipws >> currentChar;
            }

            switch (state)
            {
                case CODE:
                    currentItem = new MetaDataItem();
                    cycleString = "";
                    currentItem->code = currentChar;
                    file >> std::skipws >> currentChar;
                    if(currentChar != '{')
                    {
                        throw std::runtime_error("Metadata code \"" + string(1, currentItem->code) + "\" not followed by a {");
                    }
                    state = DESCRIPTOR;
                    break;
                case DESCRIPTOR:
                    if(currentChar == '}')
                    {
                        state = CYCLE;
                        break;
                    }

                    currentItem->descriptor += currentChar;
                    break;
                case CYCLE:
                    if(currentChar == ';' || currentChar == '.')
                    {
                        if(cycleString == "")
                        {
                            throw std::runtime_error("No metadata cycle was provided");
                        }
                        currentItem->cycle = getCycleFromString(cycleString);
                        retMetaDataItems.push_back(*currentItem);
                        delete currentItem;

                        if(currentChar == ';')
                        {
                            // Check for the next code
                            state = CODE;
                        } else
                        {
                            // "." found and metadata is over
                            finished = true;
                        }
                        break;
                    }
                    cycleString += currentChar;
                    break;
                default:
                    throw std::runtime_error("Metadata scanner state \"" + std::to_string(state) + "\" is not handled in MetaData tokenizeMetaData");
            }

        }
        // Remove the final \n
        char end_line;
        file.get(end_line);
    } catch(std::exception & e)
    {
        if(currentItem != NULL)
        {
            delete currentItem;
        }
        throw;
    }

    return retMetaDataItems;
}

int MetaData::getCycleFromString(string cycleString)
{
    int cycle;
    try
    {
        cycle = Utils::stoiNoSpace(cycleString);
    } catch (std::exception e)
    {
        throw std::runtime_error("Metadata cycle string \"" + cycleString + "\" could not be converted to an int");
    }
    return cycle;
}

void MetaData::printCycleTimes(ConfigFile & configFile, Logger & logger)
{
    
    const std::map<string, int> cycleMap
    {
        {MONITOR, configFile.monitorDisplayTime},
        {RUN, configFile.processorCycleTime},
        {MOUSE, configFile.mouseCycleTime},
        {HARD_DRIVE, configFile.hardDriveCycleTime},
        {KEYBOARD, configFile.keyBoardCycleTime},
        {PRINTER, configFile.printerCycleTime},
        {ALLOCATE, configFile.memoryCycleTime},
        {BLOCK, configFile.memoryCycleTime}
    };

    logger.log(std::cout) << "Meta-Data Metrics" << std::endl;
    for(size_t i = 0; i < metaDataItems.size(); i++)
    {
        metaDataItems[i].validate();
        if(metaDataItems[i].cycle > 0)
        {
            int cycleSpeed = cycleMap.at(metaDataItems[i].descriptor);
            logger.log(std::cout) << metaDataItems[i].getFormatted() << " - " << metaDataItems[i].cycle * cycleSpeed << " ms" << std::endl;
        }
    }
}