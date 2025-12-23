# ifndef __MOTOR_ASSERV_H
# define __MOTOR_ASSERV_H

# include "pid.h"
# include "motors.h"
# include "encoders.h"


/**
 * @brief Controls an encoded motor using the given PID
 * @note This function does not adjust for time, so it takes an encoder delta as the target rotation rate.
 */
void motor_drive_pid(int32_t tick_delta, const struct MotorHandle motor, const struct Encoder16Handle encoder, const struct PidSettings settings, struct PidRuntime *runtime);


# endif