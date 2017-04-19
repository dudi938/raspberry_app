#ifndef __MOTOR_CONTROL_h
#define __MOTOR_CONTROL_h

#include "stdint.h"



//define's
#define PI 3.14159
#define WHEEL_DIAMETER 10 * PI
#define ENCODER_COUNT_PER_TURN 100
#define GEAR_REDUCTION 246
#define ENCODER_TO_1_MILI_METER (uint32_t)((ENCODER_COUNT_PER_TURN * GEAR_REDUCTION) / (WHEEL_DIAMETER) / 10)

//TYPES
typedef enum
{
	etStop = 0,
	etForward,
	etBackward
}Direction;

typedef enum
{
	etRight = 0,
	etLeft
}MotorSide;

//FUNCTIONS

 void PlayRightMotor(Direction dir, uint8_t Power);
 void PlayLeftMotor(Direction dir, uint8_t Power);


#endif


