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

	// TEMP used to obtain negative values
	float multiplier;
};

/**
 * @brief Generate a position slope function with some pre-generated values
 * 
 * @note Use any accel / speed / distance / time units as long as they are coherent with each other. \n
 * Example : mm/s^2, mm/s, mm and s
 */
struct FuncPositionSlope pregen_position_slope(float accel, float top_speed, float dist);
float eval_position_slope(float t, struct FuncPositionSlope function);

# endif