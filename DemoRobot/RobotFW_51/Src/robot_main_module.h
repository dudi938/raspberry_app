#ifndef __ROBOT_MAIN_MODULE_H
#define __ROBOT_MAIN_MODULE_H

#include "stdint.h"
#include "platform.h"


#define DISTANCE_BETWEEN_SENSORS         25
#define ROBOT_DIAMETER									 400
#define ROBOT_SCOPE                      ROBOT_DIAMETER * PI
#define ONE_DEGREE_IN_MILIMETER          (ROBOT_SCOPE / 360)


#define MILI_METER_TO_INCREASE_SPEED     5       
#define SPEED_TO_INCERMENT               1
#define SPEED_TO_DECERMENT               20
#define START_SPEED                      10

#define REVERSE_SPEED                    30

#define ROTATE_SPEED                     30

#define BACK_IN_MILI_AFTER_IMPACT 400




extern bool RightWhiteLineEvent;
extern bool LeftWhiteLineEvent;
extern bool BackLeftPBEvent;
extern bool BackRightPBEvent;
extern bool FrontPBEvent;

typedef enum
{
	etRobotStop = 0,
	etForDirect,
	etForRight,
	etForLeft,
	etBackDirect,
	etBackRight,
	etBackLeft,
}RobotDirection;

typedef enum
{
	etStart = 0,
	etMoveForDirect,
	etMoveForRight,
	etMoveForLeft,
	etStoped,
	etMoveBackDirect,
	etMoveBackRight,
	etMoveBackLeft,
	etRightImpact,
	etLeftImpact,
	etDirectImpact,
	etWaitForRightSensor,
	etWaitForLeftSensor,	
	etError,
}RobotState;	


void RobotMove(RobotDirection Direction, uint8_t power);
void EmbergensyTask(void *op);
void InitEmbergensyTask(void);
void RobotStateMachine(void *op);
void InitRobotStateMachineTask(void);
double Rad2Degrees(double radian);



#endif

