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
	.kp = 0.4f,
	.kd = 0.0005f,
	.ki = 0.000f,
	.fratio = 0.2f,
	.decay = 0.2f,
	.i_lim = 10,
	.max_output = 100,
	.min_output = 0,
};

struct PidRuntime pid_translation_runtime = {};



struct PidSettings pid_rotation =
{
	.kp = 0.4f,
	.kd = 0.0005f,
	.ki = 0,
	.fratio = 0.2f,
	.decay = 0.5f,
	.i_lim = 0,
	.max_output = 100,
	.min_output = 0,
};

struct PidRuntime pid_rotation_runtime = {};



// PID de position de l'ascenseur vertical
// TODO : régler kp, kd, ki selon le comportement réel de l'ascenseur vertical
struct PidSettings pid_position_elevator_V =
{
	.kp = 2.5f,
	.kd = 0.0000f,
	.ki = 1.5f,
	.fratio = 0.2f,
	.decay = 0.2f,
	.i_lim = 15,
	.max_output = 40,
	.min_output = 0,
};

struct PidRuntime pid_position_elevator_V_runtime = {};



// PID de position de l'ascenseur horizontal
// TODO : régler kp, kd, ki selon le comportement réel de l'ascenseur horizontal
struct PidSettings pid_position_elevator_H =
{
	.kp = 2.0f,
	.kd = 0.000f,
	.ki = 0.4f,
	.fratio = 0.5f,
	.decay = 0.2f,
	.i_lim = 10,
	.max_output = 30,
	.min_output = 0,
};

struct PidRuntime pid_position_elevator_H_runtime = {};