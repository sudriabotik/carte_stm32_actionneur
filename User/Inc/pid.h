# ifndef __PID_H
# define __PID_H

/**
 * @file pid.h
 * 
 * @brief PID with all the needed extra features
 */


 /**
  * @brief Settings for the PID
  */
struct PidSettings
{
	// the coefficients
	float kp, ki, kd;

	// the limit of the contribution of each coefficient
	float p_lim, i_lim, k_lim;
	
	// the absolute maximum value of the PID's output
	float max_output;
	// the absolute minimum value of the PID's output
	float min_output;

	// The ratio used to filter the derivative (the higher the more smoothing)
	float fratio;

	// the decay of the integral
	float decay;
	
};


/**
 * @brief All the variables representing the current state of a PID
 */
struct PidRuntime
{
	float p;

	/** The total accumulated integral */
	float i;

	/** The current filtered derivative */
	float d;

	/** The output of the PID the last time it was ran */
	float last_err;
};


/**
 * @brief Resets all of the runtime values to default.
 */
void PID_reset_runtime(struct PidRuntime *runtime);


/**
 * @brief Manually sets the last value
 */
void PID_set_runtime_last_value(struct PidRuntime *runtime, float last_val);


/**
 * @brief runs the PID.
 * 
 * @note The runtine struct will be updated.
 * 
 * @param delta The delta time since the last PID run.
 */
float PID_Run(struct PidRuntime *runtime, const struct PidSettings *settings, float current_val, float target_val, float delta);


# endif // __PID_H