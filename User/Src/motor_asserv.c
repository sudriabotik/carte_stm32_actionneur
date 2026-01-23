# include "motor_asserv.h"

void motor_drive_pid(float delta_ms, float rpms, const struct MotorHandle motor, const struct Encoder16Handle encoder, const struct PidSettings settings, struct PidRuntime *runtime)
{
	float target = rpms * encoder.ticks_per_revolution;
	float drive = PID_Run(runtime, &settings, encoder.total_count_delta, target, delta_ms);
	motor_drive(motor, drive);
}