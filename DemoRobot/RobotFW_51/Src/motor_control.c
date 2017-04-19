#include "motor_control.h"
#include "gpio.h"
#include "platform.h"
#include "pwm.h"

#define PWM_CHANEL_RIGHT_MOTOR 1
#define PWM_CHANEL_LEFT_MOTOR  2


 void PlayRightMotor(Direction dir, uint8_t Power)
 {
		switch(dir)
		{
			case etStop:
			{
					gpio_set_pin(GPIO_SET_LOW,M_1_A_PORT,M_1_A_PIN);
					gpio_set_pin(GPIO_SET_LOW,M_1_B_PORT,M_1_B_PIN);
								
				  PwmSetFrequansy(0,PWM_CHANEL_RIGHT_MOTOR);
			}
			break;
			
			case etForward:
			{
					
					gpio_set_pin(GPIO_SET_HI,M_1_A_PORT,M_1_A_PIN);
					gpio_set_pin(GPIO_SET_LOW,M_1_B_PORT,M_1_B_PIN);

					PwmSetFrequansy(Power,PWM_CHANEL_RIGHT_MOTOR);				
			}
			break;
			
			case etBackward:
			{
					gpio_set_pin(GPIO_SET_LOW,M_1_A_PORT,M_1_A_PIN);
					gpio_set_pin(GPIO_SET_HI,M_1_B_PORT,M_1_B_PIN);
								
				  PwmSetFrequansy(Power,PWM_CHANEL_RIGHT_MOTOR);
			}
			break;
		}
 }
 void PlayLeftMotor(Direction dir, uint8_t Power)
 {
	 		switch(dir)
		{
			case etStop:
			{
					gpio_set_pin(GPIO_SET_LOW,M_2_A_PORT,M_2_A_PIN);
					gpio_set_pin(GPIO_SET_LOW,M_2_B_PORT,M_2_B_PIN);
				
					PwmSetFrequansy(0,PWM_CHANEL_LEFT_MOTOR);
			}
			break;
			
			case etForward:
			{
					gpio_set_pin(GPIO_SET_LOW,M_2_A_PORT,M_2_A_PIN);
					gpio_set_pin(GPIO_SET_HI,M_2_B_PORT,M_2_B_PIN);
				
					PwmSetFrequansy(Power,PWM_CHANEL_LEFT_MOTOR);
				
			}
			break;
			
			case etBackward:
			{
					gpio_set_pin(GPIO_SET_HI,M_2_A_PORT,M_2_A_PIN);
					gpio_set_pin(GPIO_SET_LOW,M_2_B_PORT,M_2_B_PIN);
				
					PwmSetFrequansy(Power,PWM_CHANEL_LEFT_MOTOR);
			}
			break;
		}
	}
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 





