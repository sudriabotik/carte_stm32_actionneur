# ifndef __ROBOT_DATA_H
# define __ROBOT_DATA_H

# include "encoders.h"
# include "motors.h"

extern struct Encoder16Handle encoder_R;
extern struct MotorHandle motor_R;

/**
 * @brief Initialize the struct handles to the various devices of the robot.
 * 
 * @note Always call this at the start of the program before accessing any of the structs in this file
 * 
 */
void robot_data_init();

# endif // __ROBOT_DATA_H