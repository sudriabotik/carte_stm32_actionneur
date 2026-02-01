# include "movement_statemachine.h"

# include <inttypes.h>
# include <math.h>

# include "tim.h"

# include "state_machine.h"

# include "robot_data.h"
# include "motor_asserv.h"
# include "pid.h"
# include "pid_config.h"
# include "recorder.h"
# include "mathfuncs.h"
# include "logicfuncs.h"



/*
#################
PRIVATE VARIABLES
#################
*/

struct MovementControl
{
	float elapsed_time;

	float speed;
	float dist;
	float accel;

	float pow_limit;

	// used as an additional argument for some states
	int32_t selector;
};

static struct MovementControl movement_control =
{
	.elapsed_time = 0,
	.dist = 0,
	.accel = 0,
	.speed = 0
};

struct FuncPositionSlope func_position_slope = {};



/*
######################
STATE MOTOR_SPEED_TEST
######################
*/

float motor_test_speed = 0.0f;

void state_motor_test_wake(struct StateMachine *state_machine, float delta_time)
{
	printf("motor test wake\n");
	motor_test_speed = -100.0f;
}

void state_motor_test_run(struct StateMachine *state_machine, float delta_time)
{
	printf("motor test running\n");
	motor_drive(motor_R, motor_test_speed);
	if (motor_test_speed < 100.0f) motor_test_speed += 0.5f;
}

void state_motor_test_stop(struct StateMachine *state_machine, float delta_time)
{
	printf("motor test end\n");
}

struct State MOVEMENT_STATE_MOTOR_TEST =
{
	.wake = state_motor_test_wake,
	.run = state_motor_test_run,
	.stop = state_motor_test_stop
};



/*
#################
STATE TRANSLATION
#################
*/

void state_translation_wake(struct StateMachine *state_machine, float delta_time)
{
	printf("performing a translation of distance %.3f mm\n", movement_control.dist);

	// reset the encoder, as such we can use the distance to zero as the travelled distance.
	Encoder16Reset(&encoder_R);
	Encoder16Reset(&encoder_L);

	// pregenerates a trapezoidal function with the correct parameters
	func_position_slope = pregen_position_slope(movement_control.accel, movement_control.speed, movement_control.dist);

	movement_control.elapsed_time = 0;
}

void state_translation_run(struct StateMachine *state_machine, float delta_time)
{
	Encoder16Update(&encoder_R);
	Encoder16Update(&encoder_L);
	movement_control.elapsed_time += delta_time;

	// state exit condition
	if (is_val_near(encoder_R.total_count, movement_control.dist, 1.0f) 
			&& is_val_near(encoder_L.total_count, movement_control.dist, 1.0f))
	{
		SM_Switch(state_machine, 0);
	}

	float distance_travelled = (encoder_R.total_count + encoder_L.total_count) / 2;
	float rotation_error = (encoder_R.total_count - encoder_L.total_count) / 2;

	float desired_position = eval_position_slope(movement_control.elapsed_time, func_position_slope);

	float motor_command_position = PID_Run(&pid_translation_runtime, &pid_translation, distance_travelled, desired_position, delta_time);
	float motor_command_rotation = PID_Run(&pid_rotation_runtime, &pid_rotation, rotation_error, 0, delta_time);

	printf("translation pid command : %2.3f\n", motor_command_position);
	printf("rotation pid command : %2.3f\n", motor_command_rotation);

	motor_drive_pid(delta_time, motor_command_position + motor_command_rotation, motor_R, encoder_R, pid_motor_R, &pid_motor_R_runtime);
	motor_drive_pid(delta_time, motor_command_position - motor_command_rotation, motor_L, encoder_L, pid_motor_L, &pid_motor_L_runtime);

	
}

void state_translation_stop(struct StateMachine *state_machine, float delta_time)
{
	printf("finished a translation of distance %.3f mm\n", movement_control.dist);

	motor_drive(motor_R, 0.0f);
	motor_drive(motor_L, 0.0f);
}

struct State MOVEMENT_STATE_MOVE_STRAIGHT =
{
	.wake = state_translation_wake,
	.run = state_translation_run,
	.stop = state_translation_stop
};



/*
##########
STATE HOLD
##########
*/

void state_hold_wake(struct StateMachine *state_machine, float delta_time)
{
	printf("holding the robot in place");

	// reset the encoder, as such we can use the distance to zero as the travelled distance.
	Encoder16Reset(&encoder_R);
	Encoder16Reset(&encoder_L);

	// pregenerates a trapezoidal function with the correct parameters
	func_position_slope = pregen_position_slope(movement_control.accel, movement_control.speed, movement_control.dist);

	// resets all used PID runtimes
	PID_reset_runtime(&pid_translation_runtime);
	PID_reset_runtime(&pid_rotation_runtime);
	PID_reset_runtime(&pid_motor_R_runtime);
	PID_reset_runtime(&pid_motor_L_runtime);

	movement_control.elapsed_time = 0;
}

void state_hold_run(struct StateMachine *state_machine, float delta_time)
{
	Encoder16Update(&encoder_R);
	Encoder16Update(&encoder_L);

	movement_control.elapsed_time += delta_time;

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

void state_hold_stop(struct StateMachine *state_machine, float delta_time)
{
	printf("stopped holding the robot\n");

	motor_drive(motor_R, 0.0f);
	motor_drive(motor_L, 0.0f);
}

struct State MOVEMENT_STATE_HOLD =
{
	.wake = state_hold_wake,
	.run = state_hold_run,
	.stop = state_hold_stop
};



/*
#################
STATE RECALIBRATE
#################
*/

void state_recalibrate_wake(struct StateMachine *state_machine, float delta_time)
{
	printf("recalibrating the position\n");

	// reset the encoder, as such we can use the distance to zero as the travelled distance.
	Encoder16Reset(&encoder_R);
	Encoder16Reset(&encoder_L);

	// resets all used PID runtimes
	PID_reset_runtime(&pid_translation_runtime);
	PID_reset_runtime(&pid_rotation_runtime);
	PID_reset_runtime(&pid_motor_R_runtime);
	PID_reset_runtime(&pid_motor_L_runtime);

	// reinitializes the elapsed time
	movement_control.elapsed_time = 0;
}

void state_recalibrate_run(struct StateMachine *state_machine, float delta_time)
{
	Encoder16Update(&encoder_R);
	Encoder16Update(&encoder_L);

	movement_control.elapsed_time += delta_time;

	motor_drive_pid(delta_time, movement_control.speed, motor_R, encoder_R, pid_motor_R, &pid_motor_R_runtime);
	motor_drive_pid(delta_time, movement_control.speed, motor_L, encoder_L, pid_motor_L, &pid_motor_L_runtime);

	// exit condition
	if ((fabs(pid_motor_L_runtime.i) + fabs(pid_motor_R_runtime.i)) / 2 >= movement_control.dist) SM_Switch(state_machine, 0);
}

void state_recalibrate_stop(struct StateMachine *state_machine, float delta_time)
{
	printf("position recalibrated\n");

	motor_drive(motor_R, 0.0f);
	motor_drive(motor_L, 0.0f);
}

struct State MOVEMENT_STATE_RECALIBRATE =
{
	.wake = state_recalibrate_wake,
	.run = state_recalibrate_run,
	.stop = state_recalibrate_stop
};



/*
###########################
STATEMACHINE CORE FUNCTIONS
###########################
*/

struct StateMachine movement_statemachine = {.currentState = 0, .scheduledSwitch = 0};

static void MSM_switch(struct State *state)
{
	SM_Switch(&movement_statemachine, state);
}

void MSM_update(float delta_time_ms)
{
	SM_Run(&movement_statemachine, delta_time_ms);
}



/*
#######################################
STATEMACHINE PUBLIC INTERFACE FUNCTIONS
#######################################
*/

int MSM_busy() {return movement_statemachine.currentState != 0;}

int MSM_begin_hold()
{
	MSM_switch(&MOVEMENT_STATE_HOLD);

	return 0;
}

int MSM_begin_translation(float distance, float speed, float acceleration)
{
	// stores all the movement parameter so the statemachine can refer to them
	movement_control.dist = distance;
	movement_control.speed = speed;
	movement_control.accel = acceleration;

	// switches to the correct state
	MSM_switch(&MOVEMENT_STATE_MOVE_STRAIGHT);

	return 0;
}

int MSM_begin_recalibration(float motor_rps, float stop_threshold, float power_limit, enum Facing facing);


# ifdef MOVEMENT_STATEMACHINE_TEST

void MSM_test_motors()
{
	MSM_switch(&MOVEMENT_STATE_MOTOR_TEST);
}

# endif
