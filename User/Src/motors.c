# include "motors.h"


void motor_drive(struct MotorHandle handle, int16_t speed)
{
	if (speed > 0)
	{
		handle.pwm_ccr_f = floor((handle.pwm_arr_f * (speed)) / 100.0f);
		handle.pwm_ccr_b = 0;
	}
	
}