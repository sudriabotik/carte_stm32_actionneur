# include "robot_data.h"

struct Encoder16Handle encoder_R;
struct MotorHandle motor_R;

struct Encoder16Handle encoder_L;
struct MotorHandle motor_L;

void robot_data_init()
{
	// encoder R
	encoder_R.tim_counter = &TIM3->CNT;
	encoder_R.close_distance = 3000u;
	Encoder16ResetMiddle(&encoder_R);

	// motor R
	motor_R.pwm_arr = (uint32_t)TIM1->ARR;
	motor_R.pwm_ccr = &TIM1->CCR1;

	// encoder L
	encoder_L.tim_counter = &TIM4->CNT;
	encoder_L.close_distance = 3000u;
	Encoder16ResetMiddle(&encoder_L);

	// motor L
	motor_L.pwm_arr = (uint32_t)TIM1->ARR;
	motor_L.pwm_ccr = &TIM1->CCR4;

}