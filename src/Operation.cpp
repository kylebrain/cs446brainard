#include "Operation.h"

Operation::Operation(MetaDataItem _item, const MetaData & metaData)
{
    item = _item;
    cycleTime = metaData.cycleMap.at(item.descriptor) * item.cycle;
}