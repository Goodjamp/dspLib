#include "stdint.h"
#include "math.h"
#include "stdio.h"
#include "string.h"

#include "DspGeneric.h"
#include "FirFilter.h"

static inline float dspCalcSincFirLpCoeff(FirFilterConfig config, uint32_t index)
{
    float arg =  2 * S_PI * (1 / (config.fs)) * (index - ((float)config.q / 2) + 0.5F) * config.df;
    return sinf(arg) / arg;
}

static inline float dspCalcBlackmanCoef(FirFilterConfig config, uint32_t index){
    return 0.42F - 0.5F * cosf(2 * S_PI * index / (config.q - 1))
                + 0.08F * cosf(4 * S_PI * index / (config.q - 1));
}


static inline float dspCalchHammingCoef(FirFilterConfig config, uint32_t index){
    return 0.54F - 0.46F * cosf(2 * S_PI * index / (config.q - 1));
}

static inline float dspCalcNuttallCoef(FirFilterConfig config, uint32_t index){
    return 0.355768F - 0.48829F * cosf(2 * S_PI * index / (config.q - 1))
                    + 0.14128F * cosf(4 * S_PI * index / (config.q - 1))
                    - 0.01168F * cosf(6 * S_PI * index / (config.q - 1));
}

static float dspCalcCoeff(FirFilterConfig filtrConfig, uint32_t index)
{
    float coeff = 0;
    switch(filtrConfig.type) {
    case LOW_PATH:
        coeff = dspCalcSincFirLpCoeff(filtrConfig, index);
        break;
    case BAND_PATH: break;
    case HIGH_PATH: break;
    default: return false;
    }

    switch(filtrConfig.windowType) {
    case WITHOUT_WINDOW: break;
    case BUTTERWORTH: break;
    case BLACKMAN:
        coeff *= dspCalcBlackmanCoef(filtrConfig, index);
        break;
    case HAMMING:
        coeff *= dspCalchHammingCoef(filtrConfig, index);
        break;
    case NUTTALL:
        coeff *= dspCalcNuttallCoef(filtrConfig, index);
        break;
    default: break;
    }

    return coeff;
}

bool firFilterInit(FirFilterHandler *handler,
                   FirFilterConfig filtrConfig,
                   int32_t coeffBuff[],
                   int32_t buff[],
                   uint32_t maxInput)
{
    /******************Calculate scaling coefficient******************/
    float   summCoeffNegative = 0;
    float   summCoeffPositive = 0;
    if(!handler) {
        return false;
    }
    for(uint32_t k = 0; k < filtrConfig.q; k++) {
        float coeff = dspCalcCoeff(filtrConfig, k);
        if(coeff >= 0) {
            summCoeffPositive += coeff;
        } else {
            summCoeffNegative += coeff;
        }
    }
    summCoeffNegative *= -1.0;
    summCoeffNegative += 1.0;
    summCoeffPositive += 1.0;
    if(summCoeffNegative >= summCoeffPositive) {
        summCoeffPositive = summCoeffNegative;
    }
    handler->scallingCoeff = (int32_t)(MAX_INT32 / (maxInput * summCoeffPositive));
    /*************Scaling filter coefficients******************************/
    for(uint32_t k = 0; k < filtrConfig.q; k++) {
        coeffBuff[k] = (int32_t)(dspCalcCoeff(filtrConfig, k) * handler->scallingCoeff);
    }
    handler->scallingCoeff = (int32_t)(handler->scallingCoeff * 2.0 * S_PI);
    /***********Initializations filter handler*****************************/
    handler->q     = filtrConfig.q;
    handler->first = buff;
    handler->last  = &buff[handler->q - 1];
    handler->write = handler->first;
    handler->coeff = coeffBuff;
    memset((uint8_t*)(handler->first), 0, (handler->q) * sizeof(uint32_t));
    int32_t temp;
    for(uint32_t k = 0; k < handler->q / 2; k++) {
        temp = handler->coeff[k];
        handler->coeff[k] = handler->coeff[handler->q - 1 - k];
        handler->coeff[handler->q - 1 - k] = temp;
    }

    return true;
}

bool firFilterReset(FirFilterHandler *handler)
{
    handler->write = handler->first;
    memset(handler->first, 0, handler->q);

    return true;
}

int32_t firFilterFiltration(FirFilterHandler *handler, int32_t value)
{
    *handler->write++ = value;
    if(handler->write > handler->last) {
        handler->write = handler->first;
    };
    int32_t *valueBuff = handler->write;
    int32_t *coeff = handler->coeff;
    int32_t rez = 0;
    while(valueBuff <= handler->last) {
        rez += (*valueBuff++) * (*coeff++);
    }
    valueBuff = handler->first;
    while(valueBuff < handler->write) {
        rez += (*valueBuff++) * (*coeff++);
    }

    return (rez / handler->scallingCoeff);
}
