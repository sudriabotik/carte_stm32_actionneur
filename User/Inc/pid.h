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
	/** The total accumulated integral, after applying the coef */
	float i;
	/** The output of the PID the last time it was ran */
	float lastVal;
};


float PID_Run(struct PidRuntime *runtime, const struct PidSettings *settings, float currentVal, float targetVal);


# endif // __PID_H