#ifndef OPERATION_H_
#define OPERATION_H_

#include "MetaData.h"

class Operation
{
    public:
        MetaDataItem item;
        int cycleTime;

        Operation(MetaDataItem _item, const MetaData & metaData);
};

#endif