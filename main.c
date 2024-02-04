#include <stdio.h>
#include <stdint.h>

#include "FirFilter.h"
#include "RollingAverage.h"

#define FIR_Q   64
#define MAX_ADC 4095


int main()
{
    FirFilterConfig firFirFilterConfig = {
        .type = LOW_PATH,
        .calcType = SINC,
        .windowType = WITHOUT_WINDOW,
        .df = 100,
        .fs = 2500,
        .q  = FIR_Q,
    };
    FirFilterHandler handler;
    int32_t coeffFiltr[FIR_Q] = {0, 1, 2, 3};
    int32_t buff[FIR_Q];

    firFilterInit(&handler,
                 firFirFilterConfig,
                 coeffFiltr,
                 buff,
                 MAX_ADC);
    printf("res filtr[%u] = %u\n", 1, firFilterFiltration(&handler, 1));
    printf("res filtr[%u] = %u\n", 2, firFilterFiltration(&handler, 2));
    printf("res filtr[%u] = %u\n", 3, firFilterFiltration(&handler, 3));
    printf("res filtr[%u] = %u\n", 4, firFilterFiltration(&handler, 4));
    printf("res filtr[%u] = %u\n", 5, firFilterFiltration(&handler, 5));
    printf("res filtr[%u] = %u\n", 6, firFilterFiltration(&handler, 6));
    printf("res filtr[%u] = %u\n", 7, firFilterFiltration(&handler, 7));
    printf("res filtr[%u] = %u\n", 8, firFilterFiltration(&handler, 8));
    return 0;
}
