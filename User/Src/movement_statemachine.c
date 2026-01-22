# include "movement_statemachine.h"

# include "inttypes.h"

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
};

static struct MovementControl movement_control =
{
	.elapsed_time = 0,
	.dist = 0,
	.accel = 0,
	.speed = 0
};

struct Trapezoid trapez_function = {};



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
	trapez_function = pregen_trapezoid(movement_control.dist - 5.0f, movement_control.dist, movement_control.accel); // TEMPORARY INCORRECT CALCULATIONS

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

	float desired_position = func_trapezoid(movement_control.elapsed_time, trapez_function) + 5.0f;


	float desired_speed = PID_Run(&pid_translation_runtime, &pid_translation, encoder_R.total_count_delta, desired_speed, delta_time);

	motor_drive_pid(delta_time, , motor_R, encoder_R, pid_motor_R, drive_value);
	motor_drive_pid(motor_L, drive_value);

	printf("desired speed : %2.3f\n", desired_speed);
}

void state_translation_stop(struct StateMachine *state_machine, float delta_time)
{
	printf("finished a translation of distance %.3f mm\n", movement_control.movement_dist);

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

void MSM_move_straight(float distance, float speed, float acceleration)
{
	// stores all the movement parameter so the statemachine can refer to them
	movement_control.movement_dist = distance;
	movement_control.movement_speed = speed;
	movement_control.movement_accel = acceleration;

	// switches to the correct state
	MSM_switch(&MOVEMENT_STATE_MOVE_STRAIGHT);
}


# ifdef MOVEMENT_STATEMACHINE_TEST

void MSM_test_motors()
{
	MSM_switch(&MOVEMENT_STATE_MOTOR_TEST);
}

void MSM_test_motors_pid()
{
	MSM_switch(&MOVEMENT_STATE_MOTOR_PID_TEST);
}

# endif