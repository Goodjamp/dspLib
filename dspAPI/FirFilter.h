#ifndef __FIR_FILTER_H__
#define __FIR_FILTER_H__

#include "stdint.h"
#include "stdbool.h"

typedef enum {
    LOW_PATH,
    BAND_PATH,
    HIGH_PATH
}FirFilterType;

typedef enum {
    SINC,
}FirFilterCalcType;

typedef enum {
    WITHOUT_WINDOW,
    BUTTERWORTH,
    BLACKMAN,
    HAMMING,
    NUTTALL
}WindowType;

typedef struct FirFilterConfig {
    FirFilterType type;
    FirFilterCalcType calcType;
    WindowType windowType;
    float f0;  //central frequency, Gz
    float fs;  //sampling rate, Gz
    float df;  //band with, Gz
    uint32_t q;   // filter order
}FirFilterConfig;

typedef struct FirFilterHandler {
    int32_t *first;  // first element of rezults buff
    int32_t *last;   // last element of rezults buff
    int32_t *write;  // write pointer of buff
    int32_t *coeff;  // filter coefficients
    uint32_t q;       // filter order
    int32_t scallingCoeff;
} FirFilterHandler;

bool firFilterInit(FirFilterHandler *handler,
                     FirFilterConfig filtrConfig,
                     int32_t coeffBuff[],
                     int32_t buff[],
                     uint32_t maxInput);
bool firFilterReset(FirFilterHandler *FirFilterHandler);
int32_t firFilterFiltration(FirFilterHandler *FirFilterHandler, int32_t value);

#endif
