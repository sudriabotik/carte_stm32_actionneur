# ifndef __MOTORS_H
# define __MOTORS_H

# include <stdbool.h>
# include <stdint.h>

#include "tim.h"
#include "gpio.h"
#include "stm32g431xx.h"



/**
 * @brief Stores references and variables used to drive the motors.
 * 
 * @note members in with suffixes _f are related to the forward-driving pwm,
 * while the suffix _b is for backward-driving pwm.
 */
struct MotorHandle
{
	// a pointer to the Capture/Compare register used to modify the pwm duty cycle
	volatile uint32_t *pwm_ccr;

	// the autoreload value of the PWM counter. Used to calculate the CCR from a duty cycle.
	uint32_t pwm_arr;

	// the direction pin, and whether or not it should be inversed
	GPIO_TypeDef *dir_gpio_port;
	uint16_t dir_gpio_pin;
	bool reverse;
};

/**
 * @brief Maps the speed value to 0-100% of the control pwm duty cycle.
 * 
 * @param handle A handle to the motor to drive.
 * @param speed The value to drive the motor to. Accept values -100% to 100%.
 */
void motor_drive(struct MotorHandle handle, float speed);




# endif // __MOTORS_H