#ifndef __ROLLING_AVERAGE_H__
#define __ROLLING_AVERAGE_H__

#include "stdint.h"
#include "stdbool.h"

typedef struct RollingAverageHandler {
    int32_t cnt;  // first element of rezults buff
    int32_t maxIndex;
    bool isFull;   // last element of rezults buff
    int32_t summ;  // write pointer of buff
    int32_t average;
    int32_t *buff;
    uint32_t beginPos;
    uint32_t endPos;
} RollingAverageHandler;

bool rollingAverageInit(RollingAverageHandler *handler,
                        int32_t averageSize,
                        int32_t buff[]);
bool rollingAverageReset(RollingAverageHandler *handler);
int32_t rollingAverageAdd(RollingAverageHandler *handler,
                          int32_t value);

#endif
