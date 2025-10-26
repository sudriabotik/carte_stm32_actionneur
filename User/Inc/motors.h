# ifndef __MOTORS_H
# define __MOTORS_H

#include "tim.h"
#include "gpio.h"
#include "stm32g431xx.h"


# define PWM_REGISTER_PERIOD 1024

/*
Stores infos about how to drive the motor
*/
struct MotorHandle
{
	uint32_t* pwm_autoreload;
};


/*
Defines the description of both motors
*/
struct MotorHandle motor_R =
{

};


# endif // __MOTORS_H