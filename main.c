#include <stdio.h>
#include <stdint.h>

#include "dspAPI.h"

#define FIR_Q   1024
#define MAX_ADC 4095


int main()
{
    FilterConfig firFilterConfig = {
        .type = LOW_PATH,
        .calcType = SINC,
        .windowType = WITHOUT_WINDOW,
        .df = 10,
        .fs = 2500,
        .q  = FIR_Q,
    };
    FiltrationHandler handler;
    int32_t coeffFiltr[FIR_Q] = {0, 1, 2, 3};
    int32_t buff[FIR_Q];

    dspInitFiltr(&handler,
                 firFilterConfig,
                 coeffFiltr,
                 buff,
                 MAX_ADC);
    printf("rez filtr[%u] = %u\n", 1, dspFiltration(&handler, 1));
    printf("rez filtr[%u] = %u\n", 2, dspFiltration(&handler, 2));
    printf("rez filtr[%u] = %u\n", 3, dspFiltration(&handler, 3));
    printf("rez filtr[%u] = %u\n", 4, dspFiltration(&handler, 4));
    printf("rez filtr[%u] = %u\n", 5, dspFiltration(&handler, 5));
    printf("rez filtr[%u] = %u\n", 6, dspFiltration(&handler, 6));
    printf("rez filtr[%u] = %u\n", 7, dspFiltration(&handler, 7));
    printf("rez filtr[%u] = %u\n", 8, dspFiltration(&handler, 8));
    return 0;
}
