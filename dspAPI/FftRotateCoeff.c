#include <stdint.h>
#include "FftRotateCoeff.h"

static const int32_t re[32] = {
    1000,
    980,
    923,
    831,
    707,
    555,
    382,
    195,
    0,
    -195,
    -382,
    -555,
    -707,
    -831,
    -923,
    -980,
    -1000,
    -980,
    -923,
    -831,
    -707,
    -555,
    -382,
    -195,
    0,
    195,
    382,
    555,
    707,
    831,
    923,
    980,
};

static const int32_t im[32] = {
    0,
    -195,
    -382,
    -555,
    -707,
    -831,
    -923,
    -980,
    -1000,
    -980,
    -923,
    -831,
    -707,
    -555,
    -382,
    -195,
    0,
    195,
    382,
    555,
    707,
    831,
    923,
    980,
    1000,
    980,
    923,
    831,
    707,
    555,
    382,
    195,
};

static const FftRotateCoef rotateCoef = {
    .re = re,
    .im = im,
    .size = 32,
    .scaleCoeff = 1000
};

const FftRotateCoef *fftGetCoeff(void){
    return &rotateCoef;
}