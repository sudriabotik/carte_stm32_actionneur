# include "pid.h"

# define BYPASS_PID

float PID_Run(struct PidRuntime *runtime, const struct PidSettings *settings, float currentVal, float targetVal)
{
	float p;
	float out;

	p = targetVal - currentVal;

	/** filtered derivative */
	runtime->d = (runtime->lastVal - currentVal) * (1 - settings->fratio)
				+ runtime->d * settings->fratio;

	runtime->i = runtime->i + p * settings->ki;

	// clamp the integral
	if (runtime->i < settings->iMin) runtime->i = settings->iMin;
	if (runtime->i > settings->iMax) runtime->i = settings->iMax;

	// calculates the output
	out = p * settings->kp + runtime->i + runtime->d * settings->kd;
	runtime->lastVal = out;

	// clamp the final output
	if (out > settings->max) out = settings->max;
	else if (out < settings->min) out = settings->min;

	return out;
}