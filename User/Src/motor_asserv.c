# include "motor_asserv.h"

void motor_drive_pid(int32_t tick_delta, const struct MotorHandle motor, const struct Encoder16Handle encoder, const struct PidSettings settings, struct PidRuntime *runtime)
{
	float drive = PID_Run(runtime, &settings, encoder.total_count_delta, tick_delta);
}