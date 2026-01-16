# include "pid_config.h"

struct PidSettings pid_motor_R =
{
	.kp = 10,
	.kd = 0,
	.ki = 0,
	.fratio = 0,
	.iMax = 0,
	.iMin = 0,
	.max = 1000,
	.min = -1000
};

struct PidSettings pid_motor_L =
{
	.kp = 10,
	.kd = 0,
	.ki = 0,
	.fratio = 0,
	.iMax = 0,
	.iMin = 0,
	.max = 1000,
	.min = -1000
};



struct PidSettings pid_translation =
{
	.kp = 10,
	.kd = 0,
	.ki = 0,
	.fratio = 0,
	.iMax = 0,
	.iMin = 0,
	.max = 1000,
	.min = -1000
};

struct PidSettings pid_rotation =
{
	.kp = 10,
	.kd = 0,
	.ki = 0,
	.fratio = 0,
	.iMax = 0,
	.iMin = 0,
	.max = 1000,
	.min = -1000
};