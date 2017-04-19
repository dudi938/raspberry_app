#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f0xx_hal.h"
#include <stdint.h>
#include "platform.h"

#define DEBOUNCE_COUNTER 5

typedef enum{
	GPIO_SET_LOW,
	GPIO_SET_HI,
	GPIO_SET_TOGGLE
}GPIO_PIN_STATE;


// functions
void init_gpio(void);
void gpio_set_pin(GPIO_PIN_STATE state, GPIO_TypeDef* port,uint16_t pin);
void InitScanSensorsTask(void);
void ScanSensorsTask(void *op);




#endif


