#include "pwm.h"
#include "gpio.h"
#include "platform.h"

static TIM_HandleTypeDef htim3;

void PwmInit(void)
{	
  __TIM3_CLK_ENABLE();
	
	GPIOA->AFR[0] |= 1<<24 ;  // set AF 1 
	GPIOA->AFR[0] |= 1<<28 ;  // set AF 1 

	GPIOA->MODER |= GPIO_MODER_MODER6_1;	
	GPIOA->MODER |= GPIO_MODER_MODER7_1;
	
	TIM_ClockConfigTypeDef sClockSourceConfig;
  
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = TIMER3_PRESCALER;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = TIMER3_PERIOD;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.RepetitionCounter = TIMER3_REPETITION_COUNTER;
	HAL_TIM_Base_Init(&htim3);
		
	TIM3->CCR1 = 0;
	TIM3->CCR2 = 0;
	
	TIM3->CCER |= TIM_CCER_CC1E;
	TIM3->CCER |= TIM_CCER_CC2E;
	
	
//	// test interuupt
//	TIM3->DIER |= TIM_DIER_UIE;
//	NVIC_EnableIRQ(TIM3_IRQn);
//  NVIC_SetPriority(TIM3_IRQn,0);
	
	
		
	TIM3->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	TIM3->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;

	TIM3->CR1 |= TIM_CR1_CEN;	
	
}


void PwmSetFrequansy(uint8_t percent,uint8_t chanel)
{
	switch(chanel)
	{
		case 1:
		{			
			if(percent == 0)
			{
				TIM3->CCR1 = 0;
				return;
			}
			
			if(percent >= 100)
			{
				percent = 100;
				TIM3->CCR1 = TIM3->ARR + 1;
				return;
			}			
			
			TIM3->CCR1 = (uint32_t)(percent * (float)(TIM3->ARR / 100));
		}
		break;
		
		case 2:
		{			
			if(percent == 0)
			{
				TIM3->CCR2 = 0;
				return;
			}
			
			if(percent >= 100)
			{
				percent = 100;
				TIM3->CCR2 = TIM3->ARR + 1;
				return;
			}			
			
			TIM3->CCR2 = (uint32_t)(percent * (float)(TIM3->ARR / 100));
		}
		break;
		default:
		{
		
		}
	}
}