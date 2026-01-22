# ifndef __MOTOR_ASSERV_H
# define __MOTOR_ASSERV_H

# include "pid.h"
# include "motors.h"
# include "encoders.h"


/**
 * @brief Controls an encoded motor using the given PID
 * 
 */
void motor_drive_pid(float delta_ms, float rpms, const struct MotorHandle motor, const struct Encoder16Handle encoder, const struct PidSettings settings, struct PidRuntime *runtime);


# endif