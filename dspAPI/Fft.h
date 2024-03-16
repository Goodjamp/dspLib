#ifndef __FFT_H__
#define __FFT_H__

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    FFT_RES_OK,
    FFT_HANDLER_NULL_ERROR,
    FFT_DATA_NULL_ERROR,
    FFT_GET_MEM_UNDEFINED_ERROR,
    FFT_MALLOC_NULL_ERROR,
} FftStatus;

typedef struct {
    int32_t re;
    int32_t im;
} FftRes;

typedef uint8_t *(*GetMemCb)(uint32_t memSize);

typedef struct {
    GetMemCb getMemCb;
    uint32_t scalePow;
    uint32_t wStep;
} FftHandler;

FftStatus fftInit(FftHandler *handler, GetMemCb getMemCb);
FftStatus fftPrepare(FftHandler *handler, uint16_t size);
FftStatus fftTransform(FftHandler *handler, int16_t *inData,
                  uint16_t size, FftRes **res);

#endif