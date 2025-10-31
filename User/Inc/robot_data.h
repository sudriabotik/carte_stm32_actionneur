# ifndef __ROBOT_DATA_H
# define __ROBOT_DATA_H

# include "encoders.h"
# include "motors.h"

extern struct Encoder16Handle encoder_R;

void robot_data_init();

# endif // __ROBOT_DATA_H