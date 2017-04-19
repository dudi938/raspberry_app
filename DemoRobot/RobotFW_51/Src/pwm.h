#ifndef __PWM_H
#define __PWM_H

#include "stdint.h"

void PwmInit(void);
void PwmSetFrequansy(uint8_t percent,uint8_t chanel);

#endif

