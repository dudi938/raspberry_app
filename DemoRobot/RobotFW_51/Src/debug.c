#include "debug.h"
#include "platform.h"
#include "gpio.h"



#if 0


#define ADC_TIMEOUT 	500

void InitA2d(void)
{
		GPIO_InitTypeDef GPIO_InitStruct;

	__ADC1_CLK_ENABLE();

	
	GPIO_InitStruct.Pin = DEBUG_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(DEBUG_PORT, &GPIO_InitStruct);

	adc_calibrate();
  adc_enable();	
	
	HAL_Delay(100);
}



void adc_calibrate(void)
{

    uint32_t start_time = HAL_GetTick();
    //read internal calibration

        if ((ADC1->CR & ADC_CR_ADEN) != 0) 
        {
            ADC1->CR &= (uint32_t)(~ADC_CR_ADEN); 
        }

        ADC1->CR |= ADC_CR_ADCAL; 
        
        while ((ADC1->CR & ADC_CR_ADCAL) != 0) 
        {
            if(HAL_GetTick() > (start_time + ADC_TIMEOUT ))
            {
                return;
            }
        }
}


void adc_enable(void)
{
	
	uint32_t start_time = HAL_GetTick();
	
	ADC1->CR |= ADC_CR_ADEN; /* (1) */

	ADC1->ISR |= (1<<4);
	
	while ((ADC1->ISR & ADC_ISR_ADRDY) == 0) /* (2) */
	{
		ADC1->CR |= ADC_CR_ADEN; 
		if(HAL_GetTick() > (start_time + ADC_TIMEOUT ))
		{
			return;
		}
	}
}


uint16_t adc_get_value(uint32_t channel, uint8_t oversample) 
{
	
	uint32_t start_time = HAL_GetTick();
	uint32_t val = 0;

	//adc_enable();
	if((ADC1->ISR & ADC_ISR_ADRDY) == 0)
	{
		ADC1->CR |= ADC_CR_ADEN; 
	}
	
	ADC1->SMPR |= ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2; /* (3) */
	ADC->CCR |= ADC_CCR_VREFEN; /* (4) */
			
	ADC1->CHSELR = channel;
				
	//OVR clear
	ADC1->ISR |= (1<<4);

	int i = 0;	
	
	for( i=0; i < oversample; i++)
	{
		ADC1->CR |= ADC_CR_ADSTART; /* Start the ADC conversion */

		while ((ADC1->ISR & ADC_ISR_EOC) == 0 )
			//|| (ADC1->ISR & ADC_ISR_EOSEQ) == 0 || (ADC1->ISR & ADC_ISR_EOSMP) == 0) /* Wait end of conversion */
		{
			
			if(HAL_GetTick() > (start_time + ADC_TIMEOUT ))
			{
				return 0;
			}
		}
			val += ADC1->DR; /* Store the ADC conversion result */
	}
	
	return val/i;
}
#endif
