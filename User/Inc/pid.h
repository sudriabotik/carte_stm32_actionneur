# ifndef __PID_H
# define __PID_H

/*

*/
struct PidSettings
{
	float kp, ki, kd;
	float iMin, iMax;
};

/*

*/
struct PidRuntime
{
	float i; // the integral need to be stored between pid runs.
	float lastVal; // the output of the PID the last time it was ran.
};


float PID_Run(struct PidRuntime *runtime, const struct PidSettings *settings, float currentVal, float targetVal);


# endif // __PID_H