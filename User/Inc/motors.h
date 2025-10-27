# ifndef __MOTORS_H
# define __MOTORS_H

# include <stdbool.h>
# include <stdint.h>

#include "tim.h"
#include "gpio.h"
#include "stm32g431xx.h"

// the autoreload register of the timer used for the PWM
# define MOTOR_PWM_REGISTER_PERIOD TIM1->ARR

/*
Stores infos about how to drive the motor
*/
struct MotorHandle
{
	// a pointer to the Capture/Compare register used to modify the pwm duty cycle
	volatile uint32_t* pwm_ccr;

	GPIO_TypeDef *dir1_gpio_port;
	uint32_t dir1_gpio_num;
	GPIO_TypeDef *dir2_gpio_port;
	uint32_t dir2_gpio_num;

};


/*
Defines the description of both motors.
Does not contain valid values unless motor_handle_init has been called.
*/
extern struct MotorHandle motor_R;
extern struct MotorHandle motor_L;

void motor_handles_init();

# endif // __MOTORS_H