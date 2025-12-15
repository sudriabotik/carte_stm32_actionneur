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

# endif