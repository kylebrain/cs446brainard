#include "MetaData.h"
#include "Utils.h"
#include "ConfigFile.h"

#include <iostream>
#include <algorithm>
#include <exception>
#include <map>
#include <cctype>

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
        try
        {
            descriptorVector = META_DATA_MAP.at(toupper(code));
            throw SimError("Metadata code \'" + string(1, code) + "\' needs to be capitalized");
        } catch (std::out_of_range e)
        {
            // let error fall to the invalid error message
        }
        throw SimError("Metadata code \'" + string(1, code) + "\' is not valid");
    }

    if(std::find(descriptorVector.begin(), descriptorVector.end(), descriptor) == descriptorVector.end())
    {
        throw SimError("Metadata Descriptor \"" + descriptor + "\" is not associated with code \'" + string(1, code) + "\'");
    }

    if(cycle < 0)
    {
        throw SimError("Metadata cycle \"" + std::to_string(cycle) + "\" must be greater or equal to 0");
    }

    if((descriptor == BEGIN || descriptor == FINISH) && cycle != 0)
    {
        throw SimError("Metadata " + getFormatted() + " must have a cycle equal to 0");
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

MetaData::MetaData(Logger & _logger, ConfigFile & configFile):
    beginOperatingSystem(NULL),
    beginApplication(NULL),
    finishOperatingSystem(NULL),
    finishApplication(NULL),
    logger(_logger),
    cycleMap
    {
        {MONITOR, configFile.monitorDisplayTime},
        {RUN, configFile.processorCycleTime},
        {MOUSE, configFile.mouseCycleTime},
        {HARD_DRIVE, configFile.hardDriveCycleTime},
        {KEYBOARD, configFile.keyBoardCycleTime},
        {PRINTER, configFile.printerCycleTime},
        {ALLOCATE, configFile.memoryCycleTime},
        {BLOCK, configFile.memoryCycleTime}
    }
{
    parseMetaData(configFile.filePath);
}

void MetaData::parseMetaData(string fileName)
{
    std::ifstream metaDataFile(fileName);
    if (!metaDataFile)
    {
        throw std::invalid_argument("Could not open specified metadata file name \"" + fileName +"\"! Was it mispelled?");
    }

    if(metaDataFile.peek() == std::ifstream::traits_type::eof())
    {
        throw std::invalid_argument("Specified metadata file \"" + fileName +"\" is empty");
    }

    logger.log(std::cout) << "Meta-Data Metrics" << std::endl;

    Utils::RemoveHeader("Start Program Meta-Data Code:", metaDataFile);
    metaDataItems = tokenizeMetaData(metaDataFile);
    if(beginOperatingSystem == NULL || beginApplication == NULL || finishOperatingSystem == NULL || finishApplication == NULL)
    {
        metaDataFile.close();
        throw SimError("Metadata must have S/A {begin/finish} pairs");
    }
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
                        if(currentItem->code == '{')
                        {
                            throw SimError("Missing metadata code");
                        }
                        throw SimError("Metadata code \'" + string(1, currentItem->code) + "\' not followed by a {");
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
                            throw SimError("No metadata cycle was provided");
                        }
                        currentItem->cycle = getCycleFromString(cycleString);
                        currentItem->validate();
                        checkForBeginFinish(*currentItem);
                        printCycleTime(*currentItem);
                        retMetaDataItems.push_back(*currentItem);
                        delete currentItem;

                        if(currentChar == ';')
                        {
                            // Check for the next code
                            state = CODE;
                        } else
                        {
                            // '.' found and metadata is over
                            finished = true;
                        }
                        break;
                    }
                    cycleString += currentChar;
                    break;
                default:
                    throw SimError("Metadata scanner state \"" + std::to_string(state) + "\" is not handled in MetaData tokenizeMetaData");
            }

        }
        // Remove the final '\n'
        char end_line;
        file.get(end_line);
    } catch(SimError & e)
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
        throw SimError("Metadata cycle string \"" + cycleString + "\" could not be converted to an int");
    }
    return cycle;
}

void MetaData::checkForBeginFinish(MetaDataItem & item)
{
    item.validate();
    string moreThanOne = "More than one " + string(1, item.code) + "{" + item.descriptor + "} in the metadata file";
    string outOfOrder = string(1, item.code) + "{" + BEGIN + "} must come before " + string(1, item.code) + "{" + FINISH + "}";
    if(item.descriptor == BEGIN)
    {
        if(item.code == OPERATING_SYSTEM)
        {
            if(beginOperatingSystem != NULL)
            {
                throw SimError(moreThanOne);
            }
            beginOperatingSystem = &item;
        } else // code is APPLICATION because item was validated
        {
            if(beginApplication != NULL)
            {
                throw SimError(moreThanOne);
            }
            beginApplication = &item;
        }
    } else if(item.descriptor == FINISH)
    {
        if(item.code == OPERATING_SYSTEM)
        {
            if(finishOperatingSystem != NULL)
            {
                throw SimError(moreThanOne);
            }

            if(beginOperatingSystem == NULL)
            {
                throw SimError(outOfOrder);
            }
            finishOperatingSystem = &item;
        } else // code is APPLICATION because item was validated
        {
            if(finishApplication != NULL)
            {
                throw SimError(moreThanOne);
            }
            if(beginApplication == NULL)
            {
                throw SimError(outOfOrder);
            }
            finishApplication = &item;
        }
    }
}

void MetaData::printCycleTime(MetaDataItem & metaItem)
{
    metaItem.validate();
    if(metaItem.cycle > 0)
    {
        int cycleSpeed = cycleMap.at(metaItem.descriptor);
        logger.log(std::cout) << metaItem.getFormatted() << " - " << metaItem.cycle * cycleSpeed << " ms" << std::endl;
    }
}