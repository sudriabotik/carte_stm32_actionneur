# include "movement_statemachine.h"

# include "inttypes.h"

# include "tim.h"

# include "state_machine.h"

# include "robot_data.h"
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

	float movement_speed;
	float movement_dist;
	float movement_accel;
};

static struct MovementControl movement_control =
{
	.elapsed_time = 0,
	.movement_dist = 0,
	.movement_accel = 0,
	.movement_speed = 0
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



void state_move_straight_wake(struct StateMachine *state_machine, float delta_time)
{
	printf("moving in a straight line, distance %.3f mm\n", movement_dist); // upgrade this to show values

	// reset the encoder, as such we can use the distance to zero as the travelled distance.
	Encoder16Reset(&encoder_R);
	Encoder16Reset(&encoder_L);

	// pregenerates a trapezoidal function with the correct parameters
	line_move_trapez = pregen_trapezoid(movement_speed - 10.0f, movement_dist, movement_ramp);
}

void state_move_straight_run(struct StateMachine *state_machine, float delta_time)
{
	Encoder16Update(&encoder_R);
	Encoder16Update(&encoder_L);

	// state exit condition
	if (is_val_near(encoder_R.total_count, movement_dist, 1.0f) && is_val_near(encoder_L.total_count, movement_dist, 1.0f)) SM_Switch(state_machine, 0);

	float distance_travelled = (encoder_R.total_count + encoder_L.total_count) / 2;

	float desired_speed = func_trapezoid(distance_travelled, line_move_trapez) + 10.0f;
	float drive_value = PID_Run(&line_move_pid_runtime, &pid_translation, encoder_R.total_count_delta, desired_speed);

	motor_drive(motor_R, drive_value);
	motor_drive(motor_L, drive_value);

	printf("desired speed : %2.3f\n", desired_speed);
}

void state_move_straight_stop(struct StateMachine *state_machine, float delta_time)
{
	printf("straight movement finished\n");

	motor_drive(motor_R, 0.0f);
	motor_drive(motor_L, 0.0f);
}

struct State MOVEMENT_STATE_MOVE_STRAIGHT =
{
	.wake = state_move_straight_wake,
	.run = state_move_straight_run,
	.stop = state_move_straight_stop
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