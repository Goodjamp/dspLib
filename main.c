#include <stdio.h>
#include <stdint.h>

#include "dspAPI.h"

#define FIR_Q 128

int main()
{
    FirFilterConfig firFilterConfig = {
        .df = 10,
        .fs = 1000,
        .q  = FIR_Q,
    };
    uint32_t coeff[FIR_Q];
    getFirLpfCoeff(coeff, firFilterConfig);
    return 0;
}
