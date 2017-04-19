#include "gpio.h"
#include "platform.h"
#include "robot_main_module.h"
#include "task_queue.h"



/**************************************************
* Function name	: init_gpio
* Returns		:	
* Arg				: 
* Created by	: David G
* Date created	: ~04.08.16
* Description	: Initializes GPIO pins states
**************************************************/	


	static int WhiteLineRightSensorCount = 0;
	static int WhiteLineLeftSensorCount = 0;





void init_gpio(void)
{
	 GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();
	__GPIOF_CLK_ENABLE();

	
	GPIO_InitStruct.Pin = M_1_A_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(M_1_A_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = M_1_B_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(M_1_B_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = M_1_EN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(M_1_EN_PORT, &GPIO_InitStruct);
	
	
	
/////////////////////////////

	GPIO_InitStruct.Pin = M_2_A_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(M_2_A_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = M_2_B_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(M_2_B_PORT, &GPIO_InitStruct);
	
	
	GPIO_InitStruct.Pin = M_2_PWM_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(M_2_PWM_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = M_2_EN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(M_2_EN_PORT, &GPIO_InitStruct);
	
	
#ifdef INTERRUPT_MODE	
	
	// WHITE LINE SENSOR
	GPIO_InitStruct.Pin  = WHITE_LINE_R_SENSOR_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(WHITE_LINE_R_PORT, &GPIO_InitStruct);
	
	
	SYSCFG->EXTICR[0] &= SYSCFG_EXTICR1_EXTI3_PB;
	EXTI->IMR |= EXTI_EMR_MR3;
	EXTI->FTSR |= EXTI_EMR_MR3;
	NVIC_EnableIRQ(EXTI2_3_IRQn);
  NVIC_SetPriority(EXTI2_3_IRQn,0);
	
	
	GPIO_InitStruct.Pin =  WHITE_LINE_L_SENSOR_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(WHITE_LINE_L_PORT, &GPIO_InitStruct);
	
	
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR2_EXTI4_PB;
	EXTI->IMR |= EXTI_EMR_MR4;
	EXTI->FTSR |= EXTI_EMR_MR4;
	NVIC_EnableIRQ(EXTI4_15_IRQn);
  NVIC_SetPriority(EXTI4_15_IRQn,0);



	// PUSH SENSORS
	GPIO_InitStruct.Pin  = PUSH_SENSOR_BACK_L_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(PUSH_SENSOR_BACK_L_PORT, &GPIO_InitStruct);
	
											 
	SYSCFG->EXTICR[0] &= SYSCFG_EXTICR4_EXTI13_PB;
	EXTI->IMR |= EXTI_EMR_MR13;
	EXTI->RTSR |= EXTI_EMR_MR13;
	
	
	GPIO_InitStruct.Pin =  PUSH_SENSOR_BACK_R_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(PUSH_SENSOR_BACK_L_PORT, &GPIO_InitStruct);
	
	
	SYSCFG->EXTICR[0] &= SYSCFG_EXTICR4_EXTI14_PB;
	EXTI->IMR |= EXTI_EMR_MR14;
	EXTI->RTSR |= EXTI_EMR_MR14;

	
	GPIO_InitStruct.Pin =  PUSH_SENSOR_FRONT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(PUSH_SENSOR_FRONT_PORT, &GPIO_InitStruct);
	
	
	SYSCFG->EXTICR[0] &= SYSCFG_EXTICR4_EXTI15_PB;
	EXTI->IMR |= EXTI_EMR_MR15;
	EXTI->RTSR |= EXTI_EMR_MR15;
	
	
	NVIC_EnableIRQ(EXTI4_15_IRQn);
  NVIC_SetPriority(EXTI4_15_IRQn,0);
	
#else

	// WHITE LINE SENSOR
	GPIO_InitStruct.Pin  = WHITE_LINE_R_SENSOR_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(WHITE_LINE_R_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin =  WHITE_LINE_L_SENSOR_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(WHITE_LINE_L_PORT, &GPIO_InitStruct);
	

	// PUSH SENSORS
	GPIO_InitStruct.Pin  = PUSH_SENSOR_BACK_L_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(PUSH_SENSOR_BACK_L_PORT, &GPIO_InitStruct);
	
	
	GPIO_InitStruct.Pin =  PUSH_SENSOR_BACK_R_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(PUSH_SENSOR_BACK_L_PORT, &GPIO_InitStruct);
	

	
	GPIO_InitStruct.Pin =  PUSH_SENSOR_FRONT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(PUSH_SENSOR_FRONT_PORT, &GPIO_InitStruct);
	

#endif

}


//init Sensors task 
TaskHandle ScanSensorsTaskID;
void InitScanSensorsTask(void)
{	
	ScanSensorsTaskID = add_task(&ScanSensorsTask);
	exec_task(ScanSensorsTaskID, 1, 1, (void*) NULL);			
}

void ScanSensorsTask(void *op)
{
	static volatile  uint32_t LastRTriggerTime = 0;
	static volatile uint32_t LastLTriggerTime = 0;
	
	
	

	if((((WHITE_LINE_R_PORT->IDR & WHITE_LINE_R_SENSOR_PIN) == 0) && ((HAL_GetTick() - LastRTriggerTime) < 100)) || // trigger  * time condition 
		 ((WHITE_LINE_R_PORT->IDR & WHITE_LINE_R_SENSOR_PIN) && (WhiteLineRightSensorCount == 0))	)                   // trigger  * first trigger 
	{	
		LastRTriggerTime = HAL_GetTick();		
		
		WhiteLineRightSensorCount++;
		//right sensor
		if(WhiteLineRightSensorCount >= DEBOUNCE_COUNTER)
		{
			WhiteLineRightSensorCount = 0;
			RightWhiteLineEvent = true;
		}
	}
	else
	{
		LastRTriggerTime = HAL_GetTick();
		WhiteLineRightSensorCount = 0;
	}
	

	if((((WHITE_LINE_L_PORT->IDR & WHITE_LINE_L_SENSOR_PIN) == 0) && ((HAL_GetTick() - LastLTriggerTime) < 100)) || // trigger  * time condition 
		 ((WHITE_LINE_L_PORT->IDR & WHITE_LINE_L_SENSOR_PIN) && (WhiteLineLeftSensorCount == 0))	)                   // trigger  * first trigger 
	{	
		LastLTriggerTime = HAL_GetTick();		
		
		WhiteLineLeftSensorCount++;
		//right sensor
		if(WhiteLineLeftSensorCount >= DEBOUNCE_COUNTER)
		{
			WhiteLineLeftSensorCount = 0;
			LeftWhiteLineEvent = true;
		}
	}
	else
	{
		LastLTriggerTime = HAL_GetTick();
		WhiteLineLeftSensorCount = 0;
	}
	
	
	
	
	if((PUSH_SENSOR_BACK_L_PORT->IDR & PUSH_SENSOR_BACK_L_PIN) == PUSH_SENSOR_BACK_L_PIN)	
	{	
		BackLeftPBEvent = true;
	}
	if((PUSH_SENSOR_BACK_R_PORT->IDR & PUSH_SENSOR_BACK_R_PIN) == PUSH_SENSOR_BACK_R_PIN)	
	{
		BackRightPBEvent = true;
	}
	if((PUSH_SENSOR_FRONT_PORT->IDR & PUSH_SENSOR_FRONT_PIN) == PUSH_SENSOR_FRONT_PIN)	
	{
		FrontPBEvent = true;
	}	
}

void EXTI2_3_IRQHandler(void)
{		
	#ifdef INTERRUPT_MODE	
	if((WHITE_LINE_R_PORT->IDR & WHITE_LINE_R_SENSOR_PIN) == 0)	
	{
		//right sensor
		RightWhiteLineEvent = true;
	}
	EXTI->PR |= EXTI_EMR_MR3;
	#endif
}


void EXTI4_15_IRQHandler(void)
{	
	#ifdef INTERRUPT_MODE
	if((WHITE_LINE_L_PORT->IDR & WHITE_LINE_L_SENSOR_PIN) == 0)	
	{
		//left sensor
		LeftWhiteLineEvent = true;
	}
	if((PUSH_SENSOR_BACK_L_PORT->IDR & PUSH_SENSOR_BACK_L_PIN) == PUSH_SENSOR_BACK_L_PIN)	
	{	
		BackLeftPBEvent = true;
	}
	if((PUSH_SENSOR_BACK_R_PORT->IDR & PUSH_SENSOR_BACK_R_PIN) == PUSH_SENSOR_BACK_R_PIN)	
	{
		BackRightPBEvent = true;
	}
	if((PUSH_SENSOR_FRONT_PORT->IDR & PUSH_SENSOR_FRONT_PIN) == PUSH_SENSOR_FRONT_PIN)	
	{
		FrontPBEvent = true;
	}	
			EXTI->PR |= EXTI_EMR_MR4
							 | EXTI_EMR_MR13
							 | EXTI_EMR_MR14
							 | EXTI_EMR_MR15;
	#endif
}








void gpio_set_pin(GPIO_PIN_STATE state, GPIO_TypeDef* port,uint16_t pin)
{      
	switch (state)
	{    	
		case GPIO_SET_HI:			
		port->BSRR |= pin;
		break;
			
		case GPIO_SET_LOW:
		port->BRR |= pin;
		break;

		case GPIO_SET_TOGGLE:
		port->ODR ^= pin;
		break;
			
		default:	
		break;
	}
}


