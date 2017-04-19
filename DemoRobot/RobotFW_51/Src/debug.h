#ifndef __DEBUG_H
#define __DEBUG_H

#include "stdint.h"


void InitA2d(void);
void adc_enable(void);
void adc_calibrate(void);
uint16_t adc_get_value(uint32_t channel, uint8_t oversample) ;


#endif


