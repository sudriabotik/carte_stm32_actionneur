# include "mathfuncs.h"

# include <math.h>
# include <stdio.h>


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

float eval_position_slope_f3_overlap(float t, struct FuncPositionSlope func)
{
	return ((func.l1 * 2 * t) - ((t*t) / 2) - (func.l1*func.l1)) * func.accel;
}

struct FuncPositionSlope pregen_position_slope(float accel, float top_speed, float dist)
{
	struct FuncPositionSlope func;

	func.accel = fabs(accel);
	func.top_speed = fabs(top_speed);
	func.dist = dist;

	// first, verify if there is enough distance and acceleration to reach the max speed

	// the time length of the acceleration/deceleration parts of the function necessary to reach the top speed.
	func.l1 = top_speed/accel;

	// the distance travelled during these TWO parts.
	float d1 = (func.top_speed * func.top_speed) / (func.accel);

	// check if it is more that the requested distance.
	if (d1 > func.dist)
	{
		// no steady speed part to the function
		func.overlap = 1;

		func.l1 = sqrtf(fabs(func.dist) / func.accel);
		func.l2 = func.l1*2; // the end of the deceleration slope, after which the func becomes constant
	}
	else
	{
		// there is a steady speed part in the function
		func.overlap = 0;

		func.l2 = (fabs(dist) - (func.l1 * func.l1) * accel) / top_speed;

		func.c1 = eval_position_slope_f1(func.l1, func);
		func.c2 = eval_position_slope_f2(func.l1 + func.l2, func);
	}

	if (dist < 0) func.multiplier = -1;
	else func.multiplier = 1;

	return func;
}

float eval_position_slope(float t, struct FuncPositionSlope func)
{
	if (t < 0) return 0;

	switch (func.overlap)
	{
		case 0 :
			if (t < func.l1) return eval_position_slope_f1(t, func) * func.multiplier;
			else if (t < func.l1 + func.l2) return eval_position_slope_f2(t,func) * func.multiplier;
			else if (t < 2*func.l1 + func.l2) return eval_position_slope_f3(t,func) * func.multiplier;
			else return eval_position_slope_f3(2*func.l1 + func.l2,func) * func.multiplier;
			break;
		
		case 1 :
			if (t < func.l1) return eval_position_slope_f1(t, func) * func.multiplier;
			else if (t < func.l2) return eval_position_slope_f3_overlap(t, func) * func.multiplier;
			else return func.dist;
			break;
	}

	return 0; // should not be reached ever, but put there to avoid warnings
}