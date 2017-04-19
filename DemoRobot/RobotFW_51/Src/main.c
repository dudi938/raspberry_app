#include "stm32f0xx_hal.h"
#include "gpio.h"
#include "platform.h"
#include "timers.h"
#include "motor_control.h"
#include "debug.h"
#include "uart.h"
#include "task_queue.h"
#include "pwm.h"
#include "robot_main_module.h"
#include "stdio.h"
#include <math.h>

void SystemClock_Config(void);




int main(void)
{
  HAL_Init();
  SystemClock_Config();
	
	init_gpio();
	InitTimers();	
  InitUsart();	
	PwmInit();
	//InitEmbergensyTask();
	//InitRobotStateMachineTask();
	//InitScanSensorsTask();
	//HAL_Delay(3000);	
	
	
	event_loop();
	
	//uint8_t CurrentDegrees = 0;
	//uint32_t LastDistanceSample = TotalInMiliMeter;	
	//RobotMove(etForRight,50);	

	USART1->TDR = 0xff;
	HAL_Delay(100);
	
	
  while (1)
  {

	}			
}









void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


