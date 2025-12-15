# include "motors.h"

# include <math.h>


void motor_drive(struct MotorHandle handle, float speed)
{
	if (speed > 0)
	{
		*handle.pwm_ccr_f = (uint32_t)((handle.pwm_arr_f * (speed)) / 100.0f);
		*handle.pwm_ccr_b = 0;
	}
	else if (speed < 0)
	{
		*handle.pwm_ccr_f = 0;
		*handle.pwm_ccr_b = (uint32_t)((handle.pwm_arr_f * (speed)) / 100.0f);
	}
	else
	{
		*handle.pwm_ccr_f = 0;
		*handle.pwm_ccr_b = 0;
	}
	
}