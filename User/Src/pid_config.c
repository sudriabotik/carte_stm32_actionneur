# include "pid_config.h"

struct PidSettings pid_motor_R =
{
	.kp = 10,
	.kd = 0,
	.ki = 0,
	.fratio = 0,
	.i_lim = 0,
	.max_output = 100,
	.min_output = 5,
};

struct PidRuntime pid_motor_R_runtime = {};



struct PidSettings pid_motor_L =
{
	.kp = 10,
	.kd = 0,
	.ki = 0,
	.fratio = 0,
	.i_lim = 0,
	.max_output = 100,
	.min_output = 5,
};

struct PidRuntime pid_motor_L_runtime = {};



struct PidSettings pid_translation =
{
	.kp = 10,
	.kd = 0,
	.ki = 0,
	.fratio = 0,
	.i_lim = 0,
	.max_output = 100,
	.min_output = 5,
};

struct PidRuntime pid_translation_runtime = {};



struct PidSettings pid_rotation =
{
	.kp = 10,
	.kd = 0,
	.ki = 0,
	.fratio = 0,
	.i_lim = 0,
	.max_output = 100,
	.min_output = 5,
};

struct PidRuntime pid_rotation_runtime = {};