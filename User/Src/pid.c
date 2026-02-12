# include "pid.h"


void PID_reset_runtime(struct PidRuntime *runtime)
{
	runtime->p = 0;
	runtime->d = 0;
	runtime->i = 0;

	runtime->last_err = 0;
}


void PID_set_runtime_last_value(struct PidRuntime *runtime, float last_val)
{
	runtime->last_err = last_val;
}


float PID_Run(struct PidRuntime *runtime, const struct PidSettings *settings, float current_val, float target_val, float delta)
{
	float out;

	runtime->p = target_val - current_val;

	/** filtered derivative */
	runtime->d = (runtime->last_err - runtime->p) * delta * (1 - settings->fratio)
				+ runtime->d * settings->fratio;

	runtime->i = runtime->i + runtime->p * delta;

	// decay
	if (runtime->i > settings->decay * delta) runtime->i -= settings->decay * delta;
	else if (runtime->i > 0) runtime->i = 0;
	else if (runtime->i < -settings->decay * delta) runtime->i += settings->decay * delta;
	else if (runtime->i < 0) runtime->i = 0;

	// clamp the integral
	if (runtime->i < -settings->i_lim) runtime->i = -settings->i_lim;
	if (runtime->i > settings->i_lim) runtime->i = settings->i_lim;

	// calculates the output
	out = runtime->p * settings->kp + runtime->i * settings->ki + runtime->d * settings->kd;

	// clamp the final output
	if (out > settings->max_output) out = settings->max_output;
	else if (out < -settings->max_output) out = -settings->max_output;

	if (out >= 0 && out < settings->min_output) out = settings->min_output;
	else if (out <= 0 && out > -settings->min_output) out = -settings->min_output;

	runtime->last_err = runtime->p;

	return out;
}
