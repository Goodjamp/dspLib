#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "RollingAverage.h"

bool rollingAverageInit(RollingAverageHandler *handler,
                        int32_t averageSize,
                        int32_t buff[])
{
    if(!handler) {
        return false;
    }
    if(averageSize == 0) {
        return false;
    }
    handler->average = 0;
    handler->cnt  = averageSize;
    handler->isFull = false;
    handler->summ = 0;
    handler->buff = buff;
    handler->beginPos = 0;
    handler->endPos = 1;
    memset((uint8_t*)buff, 0, averageSize * sizeof(int32_t));
    return true;
}

bool rollingAverageReset(RollingAverageHandler *handler)
{
    if(!handler) {
        return false;
    }
    handler->average = 0;
    handler->summ = 0;
    handler->beginPos = 0;
    handler->endPos = 1;
    memset((uint8_t*)handler->buff, 0, handler->cnt * sizeof(int32_t));
    return true;
}

int32_t rollingAverageAdd(RollingAverageHandler *handler,
                          int32_t value)
{
    handler->summ -= handler->buff[handler->endPos];
    handler->summ += value;
    handler->endPos++;
    handler->endPos &= handler->maxIndex;
    handler->beginPos++;
    handler->beginPos &= handler->maxIndex;
    return handler->average = handler->summ / handler->cnt;
}