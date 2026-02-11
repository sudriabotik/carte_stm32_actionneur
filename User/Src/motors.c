# include "motors.h"

# include <math.h>


void motor_drive(struct MotorHandle handle, float speed)
{
	if ((speed < 0) ^ (handle.reverse)) HAL_GPIO_WritePin(handle.dir_gpio_port, handle.dir_gpio_pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(handle.dir_gpio_port, handle.dir_gpio_pin, GPIO_PIN_RESET);
	
	if (speed < -50.0f) speed = -50.0f;
	else if (speed > 50.0f) speed = 50.0f;
	*handle.pwm_ccr = (uint32_t)((handle.pwm_arr * (fabs(speed))) / 100.0f);
}