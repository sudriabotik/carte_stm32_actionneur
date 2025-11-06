# include "pid.h"


float PID_Run(struct PidRuntime *runtime, const struct PidSettings *settings, float currentVal, float targetVal)
{
	float p, d;
	float out;

	p = targetVal - currentVal;
	d = runtime->lastVal - currentVal;
	runtime->i = runtime->i + p * settings->ki;
	if (runtime->i < settings->iMin) runtime->i = settings->iMin;
	if (runtime->i > settings->iMax) runtime->i = settings->iMax;

	out = p * settings->kp + runtime->i + d * settings->kd;
	runtime->lastVal = out;

	return out;
}