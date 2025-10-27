# include "motors.h"

struct MotorHandle motor_R = {};
struct MotorHandle motor_L = {};

void motor_handles_init()
{
	motor_R.pwm_ccr = &TIM1->CCR2;
	motor_R.dir1_gpio_port = GPIOB;
	motor_R.dir2_gpio_port = GPIOB;
	motor_R.dir1_gpio_num = 4;
	motor_R.dir2_gpio_num = 5;
}