# ifndef __PID_H
# define __PID_H

/*

*/
struct PidSettings
{
	double kp, ki, kd;
	double iMin, iMax;
};

/*

*/
struct PidRuntime
{
	double i; // the integral need to be stored between pid runs.
	double lastVal; // the output of the PID the last time it was ran.
};


double PID_Run(struct PidRuntime *runtime, const struct PidSettings *settings, double currentVal, double targetVal);


# endif // __PID_H