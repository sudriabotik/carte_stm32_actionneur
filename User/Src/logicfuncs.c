# include "logicfuncs.h"

int is_val_near(float val, float target, float threshold)
{
	return val > target - threshold && val < target + threshold;
}