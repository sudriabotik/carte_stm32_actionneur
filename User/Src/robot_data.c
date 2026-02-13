# include "robot_data.h"


const float ROBOT_WHEEL_DIAMETER = 20.0f;
const float ROBOT_ENCODER_WHEEL_DIAMETER = 62.5f;
const float ROBOT_WHEELBASE_WIDTH = 300.0f;

const float ROBOT_COEF_TRANSLATION_DISTANCE = 1.0f;
const float ROBOT_COEF_WHEEL_DIFFERENCE = 1.0f;
const float ROBOT_COEF_ROTATION = 1.0f;

struct Encoder16Handle encoder_R;
struct MotorHandle motor_R;

struct Encoder16Handle encoder_L;
struct MotorHandle motor_L;

void robot_data_init()
{
	// encoder R
	encoder_R.tim_counter = &TIM3->CNT;
	encoder_R.close_distance = 20000u;
	encoder_R.ticks_per_revolution = 14000;
	Encoder16Reset(&encoder_R);

	// motor R
	motor_R.pwm_arr = (uint32_t)TIM1->ARR;
	motor_R.pwm_ccr = &TIM1->CCR1;
	motor_R.dir_gpio_port = GPIOA;
	motor_R.dir_gpio_pin = GPIO_PIN_5;
	motor_R.reverse = 1;

	// encoder L
	encoder_L.tim_counter = &TIM4->CNT;
	encoder_L.close_distance = 20000u;
	encoder_L.ticks_per_revolution = 14000;
	Encoder16Reset(&encoder_L);

	// motor L
	motor_L.pwm_arr = (uint32_t)TIM1->ARR;
	motor_L.pwm_ccr = &TIM1->CCR4;
	motor_L.dir_gpio_port = GPIOB;
	motor_L.dir_gpio_pin = GPIO_PIN_2;
	motor_L.reverse = 1;

}
