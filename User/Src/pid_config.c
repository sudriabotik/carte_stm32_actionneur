# include "pid_config.h"

struct PidSettings pid_motor_R =
{
	.kp = 1,
	.kd = 0,
	.ki = 0.0f,
	.fratio = 0,
	.decay = 4,
	.i_lim = 110000,
	.max_output = 100,
	.min_output = 0,
};

struct PidRuntime pid_motor_R_runtime = {};



struct PidSettings pid_motor_L =
{
	.kp = 1,
	.kd = 0,
	.ki = 0.0f,
	.fratio = 0,
	.decay = 4,
	.i_lim = 110000,
	.max_output = 100,
	.min_output = 0,
};

struct PidRuntime pid_motor_L_runtime = {};



struct PidSettings pid_translation =
{
	.kp = 0.2f,
	.kd = 0.0f,
	.ki = 0,
	.fratio = 0,
	.decay = 0.5f,
	.i_lim = 0,
	.max_output = 100,
	.min_output = 0,
};

struct PidRuntime pid_translation_runtime = {};



struct PidSettings pid_rotation =
{
	.kp = 0.6f,
	.kd = 0,
	.ki = 0,
	.fratio = 0,
	.decay = 0.5f,
	.i_lim = 0,
	.max_output = 100,
	.min_output = 0,
};

struct PidRuntime pid_rotation_runtime = {};