# ifndef __PID_CONFIG_H
# define __PID_CONFIG_H

# include "pid.h"

/**
 * @brief The velocity pid of the right motor
 */
extern struct PidSettings pid_motor_R;
extern struct PidRuntime pid_motor_R_runtime;

/**
 * @brief The velocity pid of the left motor
 */
extern struct PidSettings pid_motor_L;
extern struct PidRuntime pid_motor_L_runtime;

/**
 * @brief The position PID used for translation movement
 */
extern struct PidSettings pid_translation;
extern struct PidRuntime pid_translation_runtime;

/**
 * @brief The position PID used for translation movement
 */
extern struct PidSettings pid_rotation;
extern struct PidRuntime pid_rotation_runtime;

# endif