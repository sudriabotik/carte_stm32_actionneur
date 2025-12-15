# include "mathfuncs.h"


struct Trapezoid pregen_trapezoid(float height, float base_length, float slope_length)
{
	struct Trapezoid trapez =
	{
		.half = base_length / 2,
		.height = height,
		.base_length = base_length,
		.slope = height / slope_length,
		.start_slope_end = slope_length,
		.end_slope_start = base_length - slope_length
	};

	return trapez;
}

float func_trapezoid(float t, struct Trapezoid trapez)
{
	if (t < 0) return 0;
	if (t > trapez.base_length) return 0;
	
	if (t < trapez.half / 2)
	{
		if (t < trapez.start_slope_end) return t * trapez.slope;
		else return trapez.height;
	}
	else
	{
		if (t > trapez.end_slope_start) return trapez.height - (t - trapez.end_slope_start) * trapez.slope;
		else return trapez.height;
	}
}