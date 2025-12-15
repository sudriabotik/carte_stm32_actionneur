# include "robot_data.h"

struct Encoder16Handle encoder_R;
struct MotorHandle motor_R;

void robot_data_init()
{
	// encoder R
	encoder_R.tim_counter = &TIM3->CNT;
	encoder_R.close_distance = 3000u;
	Encoder16ResetMiddle(&encoder_R);

	// motor R
	motor_R.pwm_arr_f = (uint32_t)TIM16->ARR;
	motor_R.pwm_arr_b = (uint32_t)TIM16->ARR;
	motor_R.pwm_ccr_f = &TIM16->CCR1;
	motor_R.pwm_ccr_b = &TIM16->CCR2;

	// motor_R.pwm_ccr = &TIM16->CCR1;
	// motor_R.dir_gpio_port = GPIOB; // not correct at the moment
	// motor_R.dir_gpio_num = 5; // not correct at the moment
	// HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET); // enable bridge A
	// HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1); // start the pwm
	// HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); // pin for the other direction
}