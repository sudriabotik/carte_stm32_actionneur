# include "motor_asserv.h"

void motor_drive_pid(float delta_ms, float rps, const struct MotorHandle motor, const struct Encoder16Handle encoder, const struct PidSettings settings, struct PidRuntime *runtime)
{
	float target_rpms = (rps * encoder.ticks_per_revolution) / 1000; // how many ticks to pass in 1 ms
	float drive = PID_Run(runtime, &settings, encoder.total_count_delta, target_rpms, delta_ms);
	motor_drive(motor, drive);
}