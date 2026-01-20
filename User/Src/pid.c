# include "pid.h"


void PID_reset_runtime(struct PidRuntime *runtime)
{
	runtime->d = 0;
	runtime->i = 0;

	runtime->last_val = 0;
}


void PID_set_runtime_last_value(struct PidRuntime *runtime, float last_val)
{
	runtime->last_val = last_val;
}


float PID_Run(struct PidRuntime *runtime, const struct PidSettings *settings, float current_val, float target_val, float delta)
{
	float p;
	float out;

	p = target_val - current_val;

	/** filtered derivative */
	runtime->d = (runtime->last_val - current_val) * delta * (1 - settings->fratio)
				+ runtime->d * settings->fratio;

	runtime->i = runtime->i + p * settings->ki * delta;

	// clamp the integral
	if (runtime->i < -settings->i_lim) runtime->i = -settings->i_lim;
	if (runtime->i > settings->i_lim) runtime->i = settings->i_lim;

	// calculates the output
	out = p * settings->kp + runtime->i + runtime->d * settings->kd;

	// clamp the final output
	if (out > settings->max_output) out = settings->max_output;
	else if (out < -settings->max_output) out = -settings->max_output;

	if (out >= 0 && out < settings->min_output) out = settings->min_output;
	else if (out <= 0 && out > -settings->min_output) out = -settings->min_output;

	runtime->last_val = out;

	return out;
}