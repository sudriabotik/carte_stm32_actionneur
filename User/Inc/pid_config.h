# ifndef __PID_CONFIG_H
# define __PID_CONFIG_H

# include "pid.h"

/**
 * @brief The velocity pid of the right motor
 */
extern struct PidSettings pid_motor_R;
extern struct PidSettings pid_motor_L;

extern struct PidSettings pid_line_move;

# endif