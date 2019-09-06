#include "MetaData.h"
#include "Utils.h"

#include <iostream>
#include <algorithm>
#include <exception>

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
    for(int i = 0; i < metaDataItems.size(); i++)
    {
        metaDataItems[i].validate();
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
        file.get(currentChar);
        switch (state)
        {
            case CODE:
                currentItem = new MetaDataItem();
                cycleString = "";
                currentItem->code = currentChar;
                file.get(currentChar);
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
                if(currentChar == ';')
                {
                    file.get(currentChar);
                    if (currentChar != ' ' && currentChar != '\n')
                    {
                        throw std::runtime_error("Metadata file \";\" is not followed by a space or endline");
                    }
                    currentItem->cycle = getCycleFromString(cycleString);
                    retMetaDataItems.push_back(*currentItem);
                    delete currentItem;
                    state = CODE;
                    break;
                }
                if(currentChar == '.')
                {
                    file.get(currentChar);
                    if (currentChar != '\n')
                    {
                        throw std::runtime_error("Metadata file \".\" is not followed by an endline");
                    }
                    currentItem->cycle = getCycleFromString(cycleString);
                    retMetaDataItems.push_back(*currentItem);
                    delete currentItem;
                    finished = true;
                    break;
                }
                cycleString += currentChar;
                break;
            default:
                throw std::runtime_error("Metadata scanner state \"" + std::to_string(state) + "\" is not handled in MetaData tokenizeMetaData");
        }
    }
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