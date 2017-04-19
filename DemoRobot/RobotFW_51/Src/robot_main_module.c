#include "motor_control.h"
#include "platform.h"
#include "task_queue.h"
#include "stdio.h"
#include "stm32f0xx_hal.h"
#include "robot_main_module.h"
#include <math.h>
#include "gpio.h"

#define DEBUG_US_SENSORS


 bool RightWhiteLineEvent = false;
 bool LeftWhiteLineEvent  = false;
 bool BackLeftPBEvent   = false;
 bool BackRightPBEvent    = false;
 bool FrontPBEvent         = false;
 static RobotState robotState = etStart;
 static int RobotPwmPercent = 0;
 static double ImpactDegree = 0;
 uint32_t TotalInMiliMeter	= 0;
 static uint32_t LastDistanceSample = 0;

 
 
 
//init Embergensy Task
TaskHandle EmbergensyTaskID;
void InitEmbergensyTask(void)
{		
	EmbergensyTaskID = add_task(&EmbergensyTask);
	exec_task(EmbergensyTaskID, 1, 1, (void*) NULL);			
}



//init Main state  machine Task
TaskHandle RobotStateMachineTaskID;
void InitRobotStateMachineTask(void)
{		
	RobotStateMachineTaskID = add_task(&RobotStateMachine);
	exec_task(RobotStateMachineTaskID, 1, 1, (void*) NULL);			
}



void EmbergensyTask(void *op)
{
	if
	(
			RightWhiteLineEvent == true |
			LeftWhiteLineEvent  == true |
			BackLeftPBEvent   == true |
			BackRightPBEvent    == true |
			FrontPBEvent         == true 
	)
	{
		RobotMove(etRobotStop,0);
		
		
	  RightWhiteLineEvent = false;
		LeftWhiteLineEvent  = false; 
		BackLeftPBEvent   = false; 
		BackRightPBEvent    = false; 
		FrontPBEvent         = false; 
		
	  RobotState robotState = etStoped;
		
		
//		HAL_Delay(2000);
//		RobotMove(etBackDirect,20);
//		HAL_Delay(10000);
//		RobotMove(etRobotStop,0);
		
	}
}







void RobotMove(RobotDirection Direction, uint8_t power)
{
	switch(Direction)
	{		
		case etRobotStop:
		{
		  PlayRightMotor(etStop,power);
			PlayLeftMotor(etStop,power);
		}
		break;
		case etForDirect:
		{
			PlayRightMotor(etForward,power);
			PlayLeftMotor(etForward,power);
		}
		break;
		case etForRight:
		{
			PlayRightMotor(etBackward,power);
			PlayLeftMotor(etForward,power);
		}
		break;
		case etForLeft:
		{
			PlayRightMotor(etForward,power);
			PlayLeftMotor(etBackward,power);
		}
		break;
		case etBackDirect:
		{
			PlayRightMotor(etBackward,power);
			PlayLeftMotor(etBackward,power);
		}
		break;
		case etBackRight:
		{
		
		}
		break;
		case etBackLeft:
		{
		
		}
		break;		
		
		default:
		{
		
		}	
	}	
}


void RobotStateMachine(void *op)
{	
	switch(robotState)
	{
		case etStart:
		{
			RobotPwmPercent = START_SPEED;
			robotState = etMoveForDirect;
			
			LastDistanceSample = TotalInMiliMeter;
			RobotMove(etForDirect,RobotPwmPercent);
		}
		break;
		
		case etMoveForDirect:
		{
			 if((RobotPwmPercent < 100) && (TotalInMiliMeter - LastDistanceSample) > MILI_METER_TO_INCREASE_SPEED)
			 {
					LastDistanceSample = TotalInMiliMeter;
					RobotPwmPercent += SPEED_TO_INCERMENT;
				 	RobotMove(etForDirect,RobotPwmPercent);
			 }			 
			 
			 if(LeftWhiteLineEvent == true && RightWhiteLineEvent == true)
			 {
					robotState = etDirectImpact;
			 }
			 else if(LeftWhiteLineEvent == true)
			 {
				  LastDistanceSample = TotalInMiliMeter;
				 	robotState = etWaitForRightSensor;
			 }
			 else if(RightWhiteLineEvent == true)
			 {	
				  LastDistanceSample = TotalInMiliMeter;			 	
				 	robotState = etWaitForLeftSensor;
			 }	
			 
#ifdef DEBUG_US_SENSORS
			 if(FrontPBEvent == true)
			 {
					robotState = etDirectImpact;
					FrontPBEvent = false;
			 }	
#endif			 
		}
		break;	

		case etWaitForRightSensor:
		{
				if(RightWhiteLineEvent == true)
				{				
					  // tow sensors on the white line...
						
					  // calc the degreess of the impact
						ImpactDegree  = Rad2Degrees(atan((double)((TotalInMiliMeter - LastDistanceSample) / (double)DISTANCE_BETWEEN_SENSORS)));
					
					  // stop the robot
						while(RobotPwmPercent > 0)
						{
							 if(TotalInMiliMeter - LastDistanceSample > 2)
							 {
								 LastDistanceSample = TotalInMiliMeter;
								 RobotPwmPercent -= SPEED_TO_DECERMENT;
								 RobotMove(etForDirect,RobotPwmPercent);	
							 }
						 }
						RobotMove(etRobotStop,0);	
						 
						RightWhiteLineEvent = false;
						LeftWhiteLineEvent = false;
						
						if(ImpactDegree > 15)
						{
							robotState = etLeftImpact;
						}
						else
						{
							 robotState = etDirectImpact;
						}
				}
		}
		break;
		
		case etWaitForLeftSensor:
		{
				if(LeftWhiteLineEvent == true)
				{
					//debug 
//					RobotMove(etRobotStop,0);	
					
					 // tow sensors on the white line...
						
					// calc the degreess of the impact
					ImpactDegree  = Rad2Degrees(atan((double)((TotalInMiliMeter - LastDistanceSample) / (double)DISTANCE_BETWEEN_SENSORS)));
					
					  // stop the robot
						while(RobotPwmPercent > 0)
						{
							 if(TotalInMiliMeter - LastDistanceSample > 2)
							 {
								 LastDistanceSample = TotalInMiliMeter;
								 RobotPwmPercent -= SPEED_TO_DECERMENT;
								 RobotMove(etForDirect,RobotPwmPercent);	
							 }
						 }
						
						RobotMove(etRobotStop,0);	
						 
						RightWhiteLineEvent = false;
						LeftWhiteLineEvent = false;

						if(ImpactDegree > 15)
						{
							robotState = etRightImpact;
						}
						else
						{
							 robotState = etDirectImpact;
						}
				}
		}
		break;
	
		case etDirectImpact:
		{
			LeftWhiteLineEvent = false;
			RightWhiteLineEvent = false;
			FrontPBEvent = false;
			
			// STEP 1 - slowly the robot until stop...
			LastDistanceSample = TotalInMiliMeter;
			while(RobotPwmPercent > 0)
			{
				 if(TotalInMiliMeter - LastDistanceSample > 2)
				 {
					 LastDistanceSample = TotalInMiliMeter;
					 RobotPwmPercent -= SPEED_TO_DECERMENT;
					 RobotMove(etForDirect,RobotPwmPercent);	
				 }
			 }
			 RobotMove(etRobotStop,0);	
			 HAL_Delay(1000);			 
	
			// STEP 2 - go 40 [cm]	back 	
			LastDistanceSample = TotalInMiliMeter;
			RobotPwmPercent = REVERSE_SPEED;
			RobotMove(etBackDirect,RobotPwmPercent);	
			
			while(TotalInMiliMeter - LastDistanceSample < BACK_IN_MILI_AFTER_IMPACT)
			{

#ifdef DEBUG_US_SENSORS
			 if((PUSH_SENSOR_BACK_R_PORT->IDR & PUSH_SENSOR_BACK_R_PIN) == PUSH_SENSOR_BACK_R_PIN  || (PUSH_SENSOR_BACK_L_PORT->IDR & PUSH_SENSOR_BACK_L_PIN) == PUSH_SENSOR_BACK_L_PIN)
			 {
				 
				 //step 1 stop the robot
				 RobotMove(etRobotStop,0);				 
				 HAL_Delay(500);

				 //step 2 move the robot a few forward				 
				 RobotPwmPercent = ROTATE_SPEED;
			   RobotMove(etForDirect,RobotPwmPercent);	
				 HAL_Delay(1500);
				 
				 //step 3 chenge the robot direction state to forward					 
				 robotState = etMoveForDirect;
				 
				 
				 //step 4 break out from the while loop	
				 return;
			 }	
#endif

			}			
			RobotMove(etRobotStop,0);
			HAL_Delay(500);			
			
			//STEP 3 - rotate the robot ~70` right...	
			int CurrentDegrees = 0;
			LastDistanceSample = TotalInMiliMeter;
			
			RobotPwmPercent = ROTATE_SPEED;
			RobotMove(etForRight,RobotPwmPercent);	
			
			while((TotalInMiliMeter - LastDistanceSample) < (70 * ONE_DEGREE_IN_MILIMETER))
			{
				//to debug...
				CurrentDegrees = (int)(LastDistanceSample/ ONE_DEGREE_IN_MILIMETER);
			}
			RobotMove(etRobotStop,0);
			HAL_Delay(500);	

			
			//STEP 4 - start move forward
			RobotPwmPercent = START_SPEED;
			robotState = etMoveForDirect;										
		}
		break;
		
		case etLeftImpact:
		{  
			 //step 1 - make sure that is the robot stoped
			 RobotMove(etRobotStop,0);	
			 HAL_Delay(1000);			 
	
			// STEP 2 - go 40 [cm]	back 	
			LastDistanceSample = TotalInMiliMeter;
			RobotPwmPercent = REVERSE_SPEED;
			RobotMove(etBackDirect,RobotPwmPercent);	
			
			while(TotalInMiliMeter - LastDistanceSample < BACK_IN_MILI_AFTER_IMPACT)
			{ 		
#ifdef DEBUG_US_SENSORS
			 if((PUSH_SENSOR_BACK_R_PORT->IDR & PUSH_SENSOR_BACK_R_PIN) == PUSH_SENSOR_BACK_R_PIN  || (PUSH_SENSOR_BACK_L_PORT->IDR & PUSH_SENSOR_BACK_L_PIN) == PUSH_SENSOR_BACK_L_PIN)
			 {
				 
				 //step 1 stop the robot
				 RobotMove(etRobotStop,0);				 
				 HAL_Delay(500);

				 //step 2 move the robot a few forward				 
				 RobotPwmPercent = ROTATE_SPEED;
			   RobotMove(etForDirect,RobotPwmPercent);	
				 HAL_Delay(1500);
				 
				 //step 3 chenge the robot direction state to forward					 
				 robotState = etMoveForDirect;
				 
				 //step 4 break out from the while loop	
				 return;
			 }	
#endif
			}			
			RobotMove(etRobotStop,0);
			HAL_Delay(500);			
			
			//STEP 3 - rotate the robot right...	
			int CurrentDegrees = 0;
			LastDistanceSample = TotalInMiliMeter;
			
			RobotPwmPercent = ROTATE_SPEED;
			RobotMove(etForRight,RobotPwmPercent);	
			
			//while((TotalInMiliMeter - LastDistanceSample) < ((90 + (90 - ImpactDegree)) * ONE_DEGREE_IN_MILIMETER))
			while((TotalInMiliMeter - LastDistanceSample) < (170 - ImpactDegree) * ONE_DEGREE_IN_MILIMETER)
			{
				//to debug...
				CurrentDegrees = (int)(LastDistanceSample/ ONE_DEGREE_IN_MILIMETER);
			}
			RobotMove(etRobotStop,0);
			HAL_Delay(500);	

			
			//STEP 4 - start move forward
			RobotPwmPercent = START_SPEED;
			RobotMove(etForDirect,RobotPwmPercent);
			robotState = etMoveForDirect;			
		}
		break;
		case etRightImpact:
		{
			RightWhiteLineEvent = false;
			
			
			// STEP 1 - slowly the robot until stop...
			LastDistanceSample = TotalInMiliMeter;
			while(RobotPwmPercent > 0)
			{
				 if(TotalInMiliMeter - LastDistanceSample > 2)
				 {
					 LastDistanceSample = TotalInMiliMeter;
					 RobotPwmPercent -= SPEED_TO_DECERMENT;
					 RobotMove(etForDirect,RobotPwmPercent);	
				 }
			 }
			 RobotMove(etRobotStop,0);	
			 HAL_Delay(1000);			 
	
			// STEP 2 - go 40 [cm]	back 	
			LastDistanceSample = TotalInMiliMeter;
			RobotPwmPercent = REVERSE_SPEED;
			RobotMove(etBackDirect,RobotPwmPercent);	
			
			while(TotalInMiliMeter - LastDistanceSample < BACK_IN_MILI_AFTER_IMPACT)
			{ }			
			RobotMove(etRobotStop,0);
			HAL_Delay(500);			
			
			//STEP 3 - rotate the robot ~45` right...	
			int CurrentDegrees = 0;
			LastDistanceSample = TotalInMiliMeter;
			
			RobotPwmPercent = ROTATE_SPEED;
			RobotMove(etForLeft,RobotPwmPercent);	
			
			//while((TotalInMiliMeter - LastDistanceSample) < ((90 + (90 - ImpactDegree)) * ONE_DEGREE_IN_MILIMETER))
			while((TotalInMiliMeter - LastDistanceSample) < (170 - ImpactDegree) * ONE_DEGREE_IN_MILIMETER)
			{
				//to debug...
				CurrentDegrees = (int)(LastDistanceSample/ ONE_DEGREE_IN_MILIMETER);
			}
			RobotMove(etRobotStop,0);
			HAL_Delay(500);	

			
			//STEP 4 - start move forward
			RobotPwmPercent = START_SPEED;
			RobotMove(etForDirect,RobotPwmPercent);
			robotState = etMoveForDirect;
		}
		break;
		
		case etStoped:
		{
				RobotMove(etRobotStop,RobotPwmPercent);
		}
		break;
		case etError:
		{
		
		}
		break;
	}
}



double Rad2Degrees(double radian)
{
	return radian * (180 / PI);
}



