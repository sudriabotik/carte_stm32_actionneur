# include "motors.h"

struct MotorHandle motor_R = {};
struct MotorHandle motor_L = {};

void motor_handles_init()
{
	// set the bridge pins high. SHould be moved elsewhere later.
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET); // enable bridge A

	// motor R
	motor_R.pwm_ccr = &TIM16->CCR1;
	motor_R.dir1_gpio_port = GPIOB;
	motor_R.dir2_gpio_port = GPIOB;
	motor_R.dir1_gpio_num = 4;
	motor_R.dir2_gpio_num = 5;
}