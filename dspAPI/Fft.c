#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "Fft.h"
#include "FftRotateCoeff.h"

bool fftInit(FftHandler *handler, GetMemCb getMemCb)
{
    handler->getMemCb = getMemCb;
}

uint8_t revers(uint8_t arg)
{
    uint8_t result = 0;

    result |= ((arg >> 7) | (arg << 7)) & 0b10000001;
    result |= ((arg >> 5) | (arg << 5)) & 0b01000010;
    result |= ((arg >> 3) | (arg << 3)) & 0b00100100;
    result |= ((arg >> 1) | (arg << 1)) & 0b00011000;

    return result;
}

static uint32_t getLog2(uint32_t arg)
{
    uint32_t log2 = 0;

    while (arg >> log2++) {
    }

    return log2 - 1;
}

void printArray(FftRes *data, uint32_t size)
{
    printf("--------------------------\n");
    for (uint32_t k = 0; k < size; k++) {
        printf("%i  ", data[k].re);
    }
    printf("\n");
    for (uint32_t k = 0; k < size; k++) {
        printf("%i  ", data[k].im);
    }
    printf("\n");
}

bool fftTransform(FftHandler *handler, int16_t *inData,
                  uint16_t size, FftRes **res)
{
    FftRes *lRes;
    const FftRotateCoef *w = fftGetCoeff();

    /*
     * The size must be pow 2
     */
    if (((size - 1) & size) != 0) {
        /*
         * The size is not pow 2, error
         */
        printf("Error size: %u\n", (size - 1) & size);
        return false;
    }

    lRes = (FftRes *)handler->getMemCb(sizeof(FftRes) * size);
    memset(*res, 0, sizeof(FftRes) * size);
    
    uint32_t log2 = 8 - getLog2(size) + 1;

    for (uint32_t k = 0; k < size; k++) {
        lRes[revers(k) >> log2].re = inData[k];
    }

    printArray(lRes, size);

    /*
     * Make first cascade calculation without multiplying of the complex  numbers
     */
    float tempFI = 0;
    float tempSI = 0;
    for (uint32_t k = 0; k < size; k += 2) {
        tempFI = lRes[k].re;
        lRes[k].re = lRes[k].re + lRes[k + 1].re;
        lRes[k + 1].re = tempFI - lRes[k + 1].re;
    }

    printArray(lRes, size);

    /*
     * The second cascad calculation also simlified
     */
    uint32_t fI;
    uint32_t sI;

    /*
     * On the current step the (lRes[...].im == 0)
     */
    for (uint32_t k = 0; k < size / 4; k++) {
        fI = k * 4;
        sI = fI + 2;
        /*
         * First item, rotator equal to:
         *      0
         *     W  = (1 + i * 0) 
         *      4
         * Calculate new value as:
         * 
         * fI: lRes[fI].re + (1 + i * 0) * lRes[sI].re = lRes[fI].re + lRes[sI].re
         * sI: lRes[sI].re - (1 + i * 0) * lRes[sI].re = lRes[fI].re - lRes[sI].re 
         */
        tempFI = lRes[fI].re;
        lRes[fI].re = lRes[fI].re + lRes[sI].re;
        lRes[sI].re = tempFI - lRes[sI].re;

        /*
         * Second item, rotator equal to:
         *      1
         *     W  = (0 - i * 1) 
         *      4
         * Calculate new value as:
         * 
         * 
         *         As we can see, the lRes[fI].re leave the same
         *           ________________________________________ 
         *          |                                        |
         *          \/                                       \/ 
         * fI: lRes[fI].re + (0 - i * 1) * lRes[sI].re = lRes[fI].re - i * lRes[sI].re
         *
         * sI: lRes[sI].re - (0 - i * 1) * lRes[sI].re = lRes[fI].re + i * lRes[sI].re
         * 
         */
        fI++;
        sI++;
        lRes[fI].im = -lRes[sI].re;
        lRes[sI].im = lRes[sI].re;
        lRes[sI].re = lRes[fI].re;
    }

    printArray(lRes, size);
    
    /*
     * Calculating the next cascades of the FFT with using complex multiplying
     */
    uint32_t wStep = w->size / 8;
    uint32_t m = size / 8;
    uint32_t sizeHalf = size / 2;
    uint32_t wI;
    for (uint32_t n = 4; n <= sizeHalf; n *= 2) { // n - the half of size of set on the current cascade 
        for (uint32_t k = 0; k < m; k++) { // m - the number of set on the current cascade
            fI = 2 * n * k;
            sI = fI + n;
            for (uint32_t i = 0; i < n; i++) {  // i - the index of item on the curent set
                wI = i * wStep;
                /*
                 * Calculate Re part 
                 */
                tempFI = lRes[fI].re;// * w->scaleCoeff;
                lRes[fI].re = tempFI + (w->re[wI] * lRes[sI].re - w->im[wI] * lRes[sI].im) / w->scaleCoeff;
                tempSI = lRes[sI].re;  // accumulate the lRes[sI].re to use for the calculation Im part
                lRes[sI].re = 2 * tempFI - lRes[fI].re;
                //lRes[fI].re /= w->scaleCoeff;
                //lRes[sI].re /= w->scaleCoeff;
 
                /*
                 * Calculate Im part
                 */
                tempFI = lRes[fI].im;// * w->scaleCoeff;
                lRes[fI].im = tempFI + (w->re[wI] * lRes[sI].im + w->im[wI] * tempSI) / w->scaleCoeff;
                lRes[sI].im = 2 * tempFI - lRes[fI].im;
                //lRes[fI].im /= w->scaleCoeff;
                //lRes[sI].im /= w->scaleCoeff;

                fI++;
                sI++;
            }
        }
        m /= 2;
        wStep /= 2;
        printArray(lRes, size);
    }

    *res = lRes;

    return true;
}
