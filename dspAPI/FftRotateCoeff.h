#include <stdint.h>

#ifndef __ROTATE_COEFF_H__
#define __ROTATE_COEFF_H__

typedef struct {
    const int32_t *re;
    const int32_t *im;
    uint32_t size;
    int32_t scaleCoeff;
} FftRotateCoef;

const FftRotateCoef *fftGetCoeff(void);

#endif
