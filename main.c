#include <stdio.h>
#include <stdint.h>

#include "FirFilter.h"
#include "RollingAverage.h"
#include "Fft.h"

#define FIR_Q   64
#define MAX_ADC 4095

void testFiltr(void)
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
}

uint8_t *fftGetMem(uint32_t memSize)
{
    static uint8_t memBuff[64];

    if (memSize > sizeof(memBuff)) {
        printf("Fft, wrong mem size\n");
        return NULL;
    }

    return memBuff;
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof(A[0]))
void testFft(void)
{
    FftHandler fftH;
    float realData[] = {0.3535, 0.3535, 0.6464, 1.0607, 0.3535, -1.0607, -1.3535, -0.3535};
    int16_t inData[ARRAY_SIZE(realData)];
    FftRes *res;
    bool result;

    /*
     * Scale input data to the integer value
     */
    for (int32_t k = 0; k < sizeof(realData) / sizeof(realData[0]); k++) {
        inData[k] = 10000 * realData[k];
    }

    result = fftInit(&fftH, fftGetMem);
    printf("fftInit result = %s\n", result ? "true" : "false");
    result = fftTransform(&fftH, inData, ARRAY_SIZE(inData), &res);
    printf("fftTransform result = %s\n", result ? "true" : "false");
}

int main()
{
    printf("Run DSP tests\n");

    testFft();

    return 0;
}
