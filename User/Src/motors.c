# include "motors.h"

# include <math.h>


void motor_drive(struct MotorHandle handle, float speed)
{
	if ((speed < 0) ^ (handle.reverse)) HAL_GPIO_WritePin(handle.gpio_port, handle.gpio_pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(handle.gpio_port, handle.gpio_pin, GPIO_PIN_RESET);
	*handle.pwm_ccr= (uint32_t)((handle.pwm_arr * (speed)) / 100.0f);
}