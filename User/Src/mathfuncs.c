# include "mathfuncs.h"

# include <math.h>


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



float eval_position_slope_f1(float t, struct FuncPositionSlope func)
{
	return ((t*t) / 2) * func.accel;
}

float eval_position_slope_f2(float t, struct FuncPositionSlope func)
{
	return func.c1 + (t - func.l1) * func.top_speed;
}

float eval_position_slope_f3(float t, struct FuncPositionSlope func) 
{
	return func.c2 + (t - func.l1 - func.l2) * func.top_speed - (((t - func.l1 - func.l2)*(t - func.l1 - func.l2)) / 2 * func.accel);
}

struct FuncPositionSlope pregen_position_slope(float accel, float top_speed, float dist)
{
	struct FuncPositionSlope func;

	func.accel = accel;
	func.top_speed = top_speed;
	func.dist = dist;

	func.l1 = top_speed/accel;
	func.l2 = (fabs(dist) - (func.l1 * func.l1) * accel) / top_speed;

	func.c1 = eval_position_slope_f1(func.l1, func);
	func.c2 = eval_position_slope_f2(func.l1 + func.l2, func);

	if (dist < 0) func.multiplier = -1;
	else func.multiplier = 1;

	return func;
}

float eval_position_slope(float t, struct FuncPositionSlope func)
{
	if (t < 0) return 0;
	else if (t < func.l1) return eval_position_slope_f1(t, func) * func.multiplier;
	else if (t < func.l1 + func.l2) return eval_position_slope_f2(t,func) * func.multiplier;
	else if (t < 2*func.l1 + func.l2) return eval_position_slope_f3(t,func) * func.multiplier;
	else return eval_position_slope_f3(2*func.l1 + func.l2,func) * func.multiplier;
}