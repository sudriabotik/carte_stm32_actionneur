#include "mv_statemachine_states.h"



# include <inttypes.h>
# define __USE_MISC
# include <math.h>
# undef __USE_MISC

# include "tim.h"

# include "state_machine.h"

# include "robot_data.h"
# include "motor_asserv.h"
# include "pid.h"
# include "pid_config.h"
# include "recorder.h"
# include "mathfuncs.h"
# include "logicfuncs.h"



struct MvStateMachine MV_STATEMACHINE;


/**
 * @brief Calculates the current relative distance travelled using the average of the encoder counts.
 * @return The current relative distance travelled, in mm.
 */
float get_avg_distance_travelled()
{
	float distance_travelled = (encoder_R.total_count + encoder_L.total_count) / 2;
	distance_travelled = (distance_travelled / encoder_R.ticks_per_revolution) * M_PI * ROBOT_ENCODER_WHEEL_DIAMETER;
	return distance_travelled;
}

/**
 * @brief Calculates the current translation speed using the average of the encoder deltas.
 * @return The current averaged speed, in rps.
 */
float get_avg_rps_speed(float delta_ms)
{
	float speed = (encoder_R.total_count_delta + encoder_L.total_count_delta) / 2;
	speed = (speed / encoder_R.ticks_per_revolution) * (1000 / delta_ms);
	return speed;
}

/**
 * @brief Calculates the current relative rotation using the difference between the encoder counts.
 * @return The current relative rotation, in radians.
 */
float get_avg_rotation_delta()
{
	float rotation_delta = (encoder_R.total_count - encoder_L.total_count) / 2;
	rotation_delta = (rotation_delta / encoder_R.ticks_per_revolution) * M_PI * ROBOT_ENCODER_WHEEL_DIAMETER;
	rotation_delta = (2 * rotation_delta) / ROBOT_WHEELBASE_WIDTH;
	return rotation_delta;
}


/*
##########
STATE HOLD
##########
*/

void state_hold_wake(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	printf("holding the robot in place");

	// reset the encoder, as such we can use the distance to zero as the travelled distance.
	Encoder16Reset(&encoder_R);
	Encoder16Reset(&encoder_L);

	// pregenerates a trapezoidal function with the correct parameters
	env->pos_slope = pregen_position_slope(env->acceleration, env->speed, env->distance);

	// resets all used PID runtimes
	PID_reset_runtime(&pid_translation_runtime);
	PID_reset_runtime(&pid_rotation_runtime);
	PID_reset_runtime(&pid_motor_R_runtime);
	PID_reset_runtime(&pid_motor_L_runtime);

	env->elapsed_time = 0;
}

void state_hold_run(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	Encoder16Update(&encoder_R);
	Encoder16Update(&encoder_L);

	env->elapsed_time += delta_time;

	float distance_travelled = (encoder_R.total_count + encoder_L.total_count) / 2;
	distance_travelled = (distance_travelled / encoder_R.ticks_per_revolution) * M_PI * ROBOT_ENCODER_WHEEL_DIAMETER;
	float rotation_error = (encoder_R.total_count - encoder_L.total_count) / 2;
	rotation_error = (rotation_error / encoder_R.ticks_per_revolution) * M_PI * ROBOT_ENCODER_WHEEL_DIAMETER; // INCORRECT, TEMP

	float motor_command_position = PID_Run(&pid_translation_runtime, &pid_translation, distance_travelled, 0, delta_time);
	float motor_command_rotation = PID_Run(&pid_rotation_runtime, &pid_rotation, rotation_error, 0, delta_time);

	//printf("translation pid command : %2.3f\n", motor_command_position);
	//printf("rotation pid command : %2.3f\n", motor_command_rotation);

	motor_drive_pid(delta_time, motor_command_position + motor_command_rotation, motor_R, encoder_R, pid_motor_R, &pid_motor_R_runtime);
	motor_drive_pid(delta_time, motor_command_position - motor_command_rotation, motor_L, encoder_L, pid_motor_L, &pid_motor_L_runtime);
}

void state_hold_stop(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	printf("stopped holding the robot\n");

	motor_drive(motor_R, 0.0f);
	motor_drive(motor_L, 0.0f);
}

struct MvState MV_STATE_HOLD =
{
	.wake = state_hold_wake,
	.run = state_hold_run,
	.stop = state_hold_stop
};

struct MvStateEnv genenv_mv_state_hold() {struct MvStateEnv tmp = {}; return tmp;}



/*
#################
STATE TRANSLATION
#################
*/

void state_translation_wake(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	printf("performing a translation of distance %.3f mm\n", env->distance);

	// reset the encoder, as such we can use the distance to zero as the travelled distance.
	Encoder16Reset(&encoder_R);
	Encoder16Reset(&encoder_L);

	// pregenerates a trapezoidal function with the correct parameters
	env->pos_slope = pregen_position_slope(env->acceleration, env->speed, env->distance);

	env->elapsed_time = 0;
}

void state_translation_run(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	Encoder16Update(&encoder_R);
	Encoder16Update(&encoder_L);
	env->elapsed_time += delta_time;

	// state exit condition
	
	
	

	float distance_travelled = (encoder_R.total_count + encoder_L.total_count) / 2;
	distance_travelled = (distance_travelled / encoder_R.ticks_per_revolution) * M_PI * ROBOT_ENCODER_WHEEL_DIAMETER;
	float rotation_error = (encoder_R.total_count - encoder_L.total_count) / 2;
	rotation_error = (rotation_error / encoder_R.ticks_per_revolution) * M_PI * ROBOT_ENCODER_WHEEL_DIAMETER; // INCORRECT, TEMP

	float desired_position = eval_position_slope(env->elapsed_time, env->pos_slope);
	

	float motor_command_position = PID_Run(&pid_translation_runtime, &pid_translation, distance_travelled, desired_position, delta_time);
	float motor_command_rotation = PID_Run(&pid_rotation_runtime, &pid_rotation, rotation_error, 0, delta_time);

	printf("derivative : %f\n", pid_translation_runtime.d);

	//printf("translation pid command : %2.3f\n", motor_command_position);
	//printf("rotation pid command : %2.3f\n", motor_command_rotation);

	motor_drive_pid(delta_time, motor_command_position + motor_command_rotation, motor_R, encoder_R, pid_motor_R, &pid_motor_R_runtime);
	motor_drive_pid(delta_time, motor_command_position - motor_command_rotation, motor_L, encoder_L, pid_motor_L, &pid_motor_L_runtime);

	if (is_val_near(distance_travelled, env->distance, 3.0f) && fabs(pid_translation_runtime.d) < 0.05f)
	{
		MSM_set_state_finished(statemachine);
	}

	
}

void state_translation_stop(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	printf("finished a translation of distance %.3f mm\n", env->distance);

	motor_drive(motor_R, 0.0f);
	motor_drive(motor_L, 0.0f);
}

struct MvState MV_STATE_TRANSLATION =
{
	.wake = state_translation_wake,
	.run = state_translation_run,
	.stop = state_translation_stop
};

struct MvStateEnv genenv_mv_state_translation(float acceleration, float speed, float distance)
{
	struct MvStateEnv tmp =
	{
		.acceleration = acceleration, // the acceleration in mm per second square
		.speed = speed, // the speed, in mm per second
		.distance = distance // the total distance to do, in mm
	};
	return tmp;
}



/*
##############
STATE ROTATION
##############
*/

void state_rotation_wake(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	printf("performing a rotation of %.3f degrees\n", env->distance);

	// reset the encoder, as such we can use the distance to zero as the travelled distance.
	Encoder16Reset(&encoder_R);
	Encoder16Reset(&encoder_L);

	// pregenerates a trapezoidal function with the correct parameters
	env->pos_slope = pregen_position_slope(env->acceleration, env->speed, env->distance);

	env->elapsed_time = 0;
}

void state_rotation_run(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	Encoder16Update(&encoder_R);
	Encoder16Update(&encoder_L);
	env->elapsed_time += delta_time;

	// state exit condition
	
	float distance_error = get_avg_distance_travelled();
	float rotation_delta = get_avg_rotation_delta();

	float desired_rotation = eval_position_slope(env->elapsed_time, env->pos_slope); // the angle in degrees we want to point at this moment in time
	
	float motor_command_position = PID_Run(&pid_translation_runtime, &pid_translation, distance_error, 0, delta_time);
	float motor_command_rotation = PID_Run(&pid_rotation_runtime, &pid_rotation, rotation_delta, desired_rotation, delta_time);

	motor_drive_pid(delta_time, motor_command_position + motor_command_rotation, motor_R, encoder_R, pid_motor_R, &pid_motor_R_runtime);
	motor_drive_pid(delta_time, motor_command_position - motor_command_rotation, motor_L, encoder_L, pid_motor_L, &pid_motor_L_runtime);

	if (is_val_near(rotation_delta, env->distance, 3.0f) && fabs(pid_rotation_runtime.d) < 0.05f)
	{
		MSM_set_state_finished(statemachine);
	}

	
}

void state_rotation_stop(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	printf("finished a rotation of %.3f degrees\n", env->distance);

	motor_drive(motor_R, 0.0f);
	motor_drive(motor_L, 0.0f);
}

struct MvState MV_STATE_ROTATION =
{
	.wake = state_rotation_wake,
	.run = state_rotation_run,
	.stop = state_rotation_stop
};

struct MvStateEnv genenv_mv_state_rotation(float acceleration, float speed, float distance)
{
	struct MvStateEnv tmp =
	{
		.acceleration = acceleration, // the acceleration in degrees per second square
		.speed = speed, // the speed in degrees per second
		.distance = distance // the total angle to do, in degrees
	};
	return tmp;
}



/*
###################
STATE RECALIBRATION
###################
*/

void state_recalibration_wake(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	printf("performing a recalibration\n");

	// reset the encoder, as such we can use the distance to zero as the travelled distance.
	Encoder16Reset(&encoder_R);
	Encoder16Reset(&encoder_L);

	// pregenerates a trapezoidal function with the correct parameters
	env->trapezoid = pregen_trapezoid(env->speed, 10000, env->speed/env->acceleration);

	env->elapsed_time = 0;
}

void state_recalibration_run(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	Encoder16Update(&encoder_R);
	Encoder16Update(&encoder_L);
	env->elapsed_time += delta_time;

	// state exit condition
	
	float current_speed = get_avg_rps_speed(delta_time);

	float desired_speed = func_trapezoid(env->elapsed_time, env->trapezoid); // the angle in degrees we want to point at this moment in time
	
	float motor_command_speed = PID_Run(&pid_translation_runtime, &pid_translation, current_speed, desired_speed, delta_time);

	motor_drive_pid(delta_time, motor_command_speed, motor_R, encoder_R, pid_motor_R, &pid_motor_R_runtime);
	motor_drive_pid(delta_time, motor_command_speed, motor_L, encoder_L, pid_motor_L, &pid_motor_L_runtime);

	if (fabs(pid_motor_L_runtime.p) > 1.0f && fabs(pid_motor_R_runtime.p) > 1.0f)
	{
		MSM_set_state_finished(statemachine);
	}

	
}

void state_recalibration_stop(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	printf("finished a recalibration\n");

	motor_drive(motor_R, 0.0f);
	motor_drive(motor_L, 0.0f);
}

struct MvState MV_STATE_RECALIBRATION =
{
	.wake = state_recalibration_wake,
	.run = state_recalibration_run,
	.stop = state_recalibration_stop
};

struct MvStateEnv genenv_mv_state_recalibration(float acceleration, float speed, enum Facing facing)
{
	struct MvStateEnv tmp =
	{
		.acceleration = acceleration, // the acceleration in degrees per second square
		.speed = speed, // the speed in degrees per second
		
		.facing = facing // on which wall the recalibration is done.
	};
	return tmp;
}