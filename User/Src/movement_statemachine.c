# include "movement_statemachine.h"

# include "inttypes.h"

# include "tim.h"

# include "robot_data.h"
# include "pid.h"
# include "pid_config.h"
# include "recorder.h"
# include "mathfuncs.h"



/*
#################
PRIVATE VARIABLES
#################
*/

static float movement_speed;
static float movement_dist;
static float movement_ramp;



/*
######################
STATE MOTOR_SPEED_TEST
######################
*/

float motor_test_speed = 0.0f;

void state_motor_test_wake(struct StateMachine *state_machine)
{
	printf("motor test wake\n");
	motor_test_speed = -100.0f;
}

void state_motor_test_run(struct StateMachine *state_machine)
{
	printf("motor test running\n");
	motor_drive(motor_R, motor_test_speed);
	if (motor_test_speed < 100.0f) motor_test_speed += 0.5f;
}

void state_motor_test_stop(struct StateMachine *state_machine)
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
####################
STATE MOTOR_PID_TEST
####################
*/

struct PidSettings speed_test_pid_settings = {.kp = 60.0f, .ki = 0.1f, .kd = 0.1f, .iMax = 500.0f, .iMin = 0.0f, .max = 1024.0f, .min = 0.0f, .fratio = 0.5f};
struct PidRuntime speed_test_pid_runtime = {};

void state_motor_pid_test_wake(struct StateMachine *state_machine)
{
	printf("motor speed test wake\n");
	motor_drive(motor_R, 0.0f);
}

void state_motor_pid_test_run(struct StateMachine *state_machine)
{
	printf("motor speed test running\n");

	float drive_value = PID_Run(&speed_test_pid_runtime, &speed_test_pid_settings, encoder_R.total_count_delta, 4);
	if (drive_value < 0) drive_value = 0;

	motor_drive(motor_R, drive_value);

	// update the recording
	struct RecordedTick tick = {.actual = (uint32_t)encoder_R.total_count_delta, .target = 4, .drive = (uint32_t)drive_value};
	recorder_append(0, tick);
}

void state_motor_pid_test_stop(struct StateMachine *state_machine)
{
	printf("motor speed test end\n");
}

struct State MOVEMENT_STATE_MOTOR_PID_TEST =
{
	.wake = state_motor_pid_test_wake,
	.run = state_motor_pid_test_run,
	.stop = state_motor_pid_test_stop
};



/*
###############
STATE LINE_MOVE
###############
*/

struct PidRuntime line_move_pid_runtime = {};
struct Trapezoid line_move_trapez = {};

void state_move_straight_wake(struct StateMachine *state_machine)
{
	printf("moving in a straight line, distance %.3f mm\n", movement_dist); // upgrade this to show values

	// reset the encoder, as such we can use the distance to zero as the travelled distance.
	Encoder16Reset(&encoder_R);
	Encoder16Reset(&encoder_L);

	// pregenerates a trapezoidal function with the correct parameters
	line_move_trapez = pregen_trapezoid(movement_speed - 10.0f, movement_dist, movement_ramp);
}

void state_move_straight_run(struct StateMachine *state_machine)
{
	Encoder16Update(&encoder_R);
	Encoder16Update(&encoder_L);

	float distance_travelled = (encoder_R.total_count + encoder_L.total_count) / 2;

	if (distance_travelled >= movement_dist) SM_Switch(state_machine, 0); // exit

	float desired_speed = func_trapezoid(distance_travelled, line_move_trapez) + 10.0f;
	float drive_value = PID_Run(&line_move_pid_runtime, &pid_line_move, encoder_R.total_count_delta, desired_speed);

	motor_drive(motor_R, drive_value);
	motor_drive(motor_L, drive_value);

	printf("desired speed : %2.3f\n", desired_speed);
}

void state_move_straight_stop(struct StateMachine *state_machine)
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

void MSM_update()
{
	SM_Run(&movement_statemachine);
}



/*
#######################################
STATEMACHINE PUBLIC INTERFACE FUNCTIONS
#######################################
*/

int MSM_busy() {return movement_statemachine.currentState != 0;}

void MSM_move_straight(float distance, float speed, float ramp_dist)
{
	movement_dist = distance;
	movement_ramp = ramp_dist;
	movement_speed = speed;

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