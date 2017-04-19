#ifndef __PLATFORM_H
#define __PLATFORM_H

#include "motor_control.h"
extern uint32_t TotalInMiliMeter;

////////////////////////////////Motor driver/////////////
                                                         
	#define M_1_A_PORT		 								GPIOB	           
	#define M_1_A_PIN			 		  					GPIO_PIN_0       
                                                         
	#define M_1_B_PORT		 								GPIOB	           
	#define M_1_B_PIN			 								GPIO_PIN_1       

	#define M_1_PWM_PORT		 							GPIOA	         
	#define M_1_PWM_PIN			 							GPIO_PIN_6  

	#define M_1_CS_PORT		 								GPIOB	           
	#define M_1_CS_PIN			 							GPIO_PIN_10       
	                                                       																										
	#define M_1_EN_PORT		 							  GPIOB	         
	#define M_1_EN_PIN			 						  GPIO_PIN_11     
	
	
	
	#define M_2_A_PORT		 								GPIOC	           
	#define M_2_A_PIN			 		  					GPIO_PIN_2       
                                                         
	#define M_2_B_PORT		 								GPIOC	           
	#define M_2_B_PIN			 								GPIO_PIN_3       
	                                                       																										
	#define M_2_PWM_PORT		 							GPIOC	         
	#define M_2_PWM_PIN			 							GPIO_PIN_4      

	#define M_2_CS_PORT		 								GPIOC	           
	#define M_2_CS_PIN			 							GPIO_PIN_1       
	                                                       																										
	#define M_2_EN_PORT		 							  GPIOC	         
	#define M_2_EN_PIN			 						  GPIO_PIN_0  
	
	                                                       																										
////////////////////////////////Motor driver/////////////
	
////////////////////////////ENCODER//////////////////////	

	#define EXT_CLOCK_SOURCE_1_PIN       GPIO_PIN_12  
	#define EXT_CLOCK_SOURCE_1_PORT      GPIOA 

	#define EXT_CLOCK_SOURCE_2_PIN       GPIO_PIN_0 
	#define EXT_CLOCK_SOURCE_2_PORT      GPIOA
	
////////////////////////////ENCODER//////////////////////	

////////////////////////////////White line sensor////////

	#define WHITE_LINE_R_SENSOR_PIN		 		GPIO_PIN_4	           
	#define WHITE_LINE_R_PORT		 		  		GPIOB
	
	#define WHITE_LINE_L_SENSOR_PIN		 		GPIO_PIN_3	           
	#define WHITE_LINE_L_PORT		 		  		GPIOB  
	
////////////////////////////////White line sensor////////

////////////////////////////////Push sensors/////////////

	#define PUSH_SENSOR_BACK_L_PIN		 		GPIO_PIN_13	           
	#define PUSH_SENSOR_BACK_L_PORT		 	  GPIOB  
	
	#define PUSH_SENSOR_BACK_R_PIN		 		GPIO_PIN_14	           
	#define PUSH_SENSOR_BACK_R_PORT		 	  GPIOB  
	
	#define PUSH_SENSOR_FRONT_PIN		 		  GPIO_PIN_15	           
	#define PUSH_SENSOR_FRONT_PORT		 	    GPIOB  
	
////////////////////////////////Push sensors/////////////
	
	
////////////////////////////I2C//////////////////////	

	#define I2C_SCL_PIN                   GPIO_PIN_6  
	#define I2C_SCL_PORT                  GPIOB 

	#define I2C_SDA_PIN                   GPIO_PIN_7  
	#define I2C_SDA_PORT                  GPIOB 
	
////////////////////////////I2C//////////////////////		
	
////////////////////////////UART//////////////////////	
	#define UART1_TX_PIN                   GPIO_PIN_9  
	#define UART1_PORT                     GPIOA 

	#define UART1_RX_PIN                   GPIO_PIN_10  
	#define UART1_PORT                     GPIOA 
////////////////////////////UART//////////////////////	
		
	
//////////////////////////////a2d//////////////////////		
//	#define DEBUG_PIN		 		              GPIO_PIN_2	           
//	#define DEBUG_PORT		 	              GPIOA  
//////////////////////////////a2d//////////////////////		
	
// TIMER1 CONSTANTS
	#define  TIMER1_PRESCALER  0
	#define  TIMER1_PERIOD  ENCODER_TO_1_MILI_METER
	#define  TIMER1_REPETITION_COUNTER 0
	
	#define  TIMER2_PRESCALER  0
	#define  TIMER2_PERIOD  ENCODER_TO_1_MILI_METER
	#define  TIMER2_REPETITION_COUNTER 0
	
	
	#define  TIMER3_PRESCALER  0
	#define  TIMER3_PERIOD  800
	#define  TIMER3_REPETITION_COUNTER 10
// END TIMER1 CONSTANTS	
	



typedef enum
{
	false = 0,
	true =  1
}bool;


#endif

