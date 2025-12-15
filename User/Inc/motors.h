# ifndef __MOTORS_H
# define __MOTORS_H

# include <stdbool.h>
# include <stdint.h>

#include "tim.h"
#include "gpio.h"
#include "stm32g431xx.h"

// the autoreload register of the timer used for the PWM
# define MOTOR_PWM_REGISTER_PERIOD TIM1->ARR

/**
 * @brief Stores references and variables used to drive the motors.
 * 
 * @note members in with suffixes _f are related to the forward-driving pwm,
 * while the suffix _b is for backward-driving pwm.
 */
struct MotorHandle
{
	// a pointer to the Capture/Compare register used to modify the pwm duty cycle
	volatile uint32_t *pwm_ccr_f, *pwm_ccr_b;

	// the autoreload value of the PWM counter. Used to calculate the CCR from a duty cycle.
	uint32_t pwm_arr_f, pwm_arr_b;
};

/**
 * @brief Maps the speed value to 0-100% of the control pwm duty cycle.
 * 
 * @param handle A handle to the motor to drive.
 * @param speed The value to drive the motor to. Accept values -100% to 100%.
 */
void motor_drive(struct MotorHandle handle, float speed);




# endif // __MOTORS_H