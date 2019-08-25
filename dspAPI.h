#ifndef __DSP_API_H__
#define __DSP_API_H__

#include "stdint.h"
#include "stdbool.h"

typedef struct FirFilterConfig {
    double f0; //central frequency, Gz
    double fs; //sampling rate, Gz
    double df; //band with, Gz
    uint32_t q;  // filter order
}FirFilterConfig;

bool getFirLpfCoeff(uint8_t coeffList, FirFilterConfig config);

#endif
