#include "stdint.h"
#include "math.h"

#include "dspAPI.h"


bool getFirLpfCoeff(uint8_t coeffList, FirFilterConfig config)
{
    double   coeff = 0;
    double   arg = 0;
    uint32_t k = 0;
    for(uint32_t k = 0; k < config.q; k++) {
        arg = 2 * M_PI * (1 / (config.fs)) * (k - ((double)config.q / 2) + 0.5) * config.df;
        coeff = sin(arg) / arg;
        printf("coef[%3.u] = %f\n", k, coeff);
    }
}
