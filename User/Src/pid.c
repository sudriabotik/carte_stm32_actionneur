# include "pid.h"


double PID_Run(struct PidRuntime *runtime, const struct PidSettings *settings, double currentVal, double targetVal)
{
	double p, d;
	double out;

	p = targetVal - currentVal;
	d = runtime->lastVal - currentVal;
	runtime->i = runtime->i + p;

	out = p * settings->kp + runtime->i * settings->ki + d * settings->kd;
	runtime->lastVal = out;

	return out;
}