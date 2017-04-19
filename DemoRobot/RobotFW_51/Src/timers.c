#include "timers.h"
#include "stm32f0xx_hal.h"
#include <stdint.h>
#include "gpio.h"
#include "platform.h"
#include "stm32f0xx_hal_tim.h"
#include "stm32f0xx.h"


static TIM_HandleTypeDef htim1;
static TIM_HandleTypeDef htim2;

void InitTimers(void)
{	
	//TIM_ClockConfigTypeDef sClockSourceConfig;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// timer 1 (clock source from port a  pin 12)
	__TIM1_CLK_ENABLE();
	
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = TIMER1_PRESCALER;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = TIMER1_PERIOD;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = TIMER1_REPETITION_COUNTER;
	HAL_TIM_Base_Init(&htim1);
	
	//config external clk pin
	TIM1->SMCR |= TIM_SMCR_SMS_0 |TIM_SMCR_SMS_1 | TIM_SMCR_SMS_2;
	TIM1->SMCR |= TIM_SMCR_TS_0 |TIM_SMCR_TS_1 | TIM_SMCR_TS_2;
	TIM1->SMCR |= TIM_SMCR_ECE;

	GPIO_InitStruct.Pin = EXT_CLOCK_SOURCE_1_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(EXT_CLOCK_SOURCE_1_PORT, &GPIO_InitStruct);	
	
	GPIOA->AFR[1] &= ~(0x00010000);
	GPIOA->AFR[1] |= 0x00020000;
	
	//enable count
	TIM1->CR1 |= TIM_CCER_CC1E;
	
		//enable "update_flag" interrupt 
	TIM1->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
	
	
	
	
	// timer 2 (clock source from port a  pin 0)
	__TIM2_CLK_ENABLE();
	  
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = TIMER2_PRESCALER;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = TIMER2_PERIOD;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.RepetitionCounter = TIMER2_REPETITION_COUNTER;
	HAL_TIM_Base_Init(&htim2);
	
	//config external clk pin
	TIM2->SMCR |= TIM_SMCR_SMS_0 |TIM_SMCR_SMS_1 | TIM_SMCR_SMS_2;
	TIM2->SMCR |= TIM_SMCR_TS_0 |TIM_SMCR_TS_1 | TIM_SMCR_TS_2;
	TIM2->SMCR |= TIM_SMCR_ECE;
	


	GPIO_InitStruct.Pin = EXT_CLOCK_SOURCE_2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(EXT_CLOCK_SOURCE_2_PORT, &GPIO_InitStruct);	
	
	GPIOA->AFR[0] &= ~(0x0000000f);
	GPIOA->AFR[0] |= 0x00000002;
	
	//enable count
	TIM2->CR1 |= TIM_CCER_CC1E;	
	
	//enable "update_flag" interrupt 
	TIM2->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM2_IRQn);
	

}


