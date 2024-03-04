#ifndef __FFT_H__
#define __FFT_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int32_t re;
    int32_t im;
} FftRes;

typedef uint8_t *(*GetMemCb)(uint32_t memSize);

typedef struct {
    GetMemCb getMemCb;
} FftHandler;

bool fftInit(FftHandler *handler, GetMemCb getMemCb);
bool fftTransform(FftHandler *handler, int16_t *inData,
                  uint16_t size, FftRes **res);

#endif