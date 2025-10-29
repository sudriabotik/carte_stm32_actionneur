# include "robot_data.h"

struct Encoder16Handle encoder_R = {};

void robot_data_init()
{
	encoder_R.tim_counter = &TIM3->CNT;
	encoder_R.close_distance = 3000;
	
	Encoder16ResetMiddle(&encoder_R);
}