# ifndef __MATH_FUNC_H
# define __MATH_FUNC_H

struct Trapezoid
{
	// the slope of the slanted side of the trapezoid
	float slope;
	// the height
	float height;
	float base_length;
	// the value that is the middle of the base side.
	float half;
	// the value at which the end slope starts.
	float end_slope_start;
	// the value at which the start slope ends.
	float start_slope_end;
};


struct Trapezoid pregen_trapezoid(float height, float base_length, float slope_length);
float func_trapezoid(float t, struct Trapezoid trapez);


/**
 * @brief The integral of a trapezoid speed command, used for position control over time.
 */
struct FuncPositionSlope
{
	/** Whether or not there is time to reach the max speed without overshooting the distance.
	 * In other words, whether or not the steady speed state is reachable.
	 */
	int overlap;

	float dist;
	float top_speed;
	float accel;

	/** The length of the two curved parts at the end of the function */
	float l1;
	/** The length of the straight part in the middle of the function */
	float l2;

	/** The final value of the end of first curved part */
	float c1;
	/** The final value at the end of the straight part */
	float c2;

	// used to output negative values if the distance is negative (TEMP)
	float multiplier;
};

/**
 * @brief Generate a position slope function with some pre-generated values
 * 
 * @note Use any accel / speed / distance / time units as long as they are coherent with each other. \n
 * Example : mm/s^2, mm/s, mm and s
 * Acceleration and speed are always positive values, while distance can be negative.
 */
struct FuncPositionSlope pregen_position_slope(float accel, float top_speed, float dist);
float eval_position_slope(float t, struct FuncPositionSlope function);

# endif