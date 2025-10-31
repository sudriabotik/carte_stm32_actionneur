# include "movement_statemachine.h"

# include "robot_data.h"
# include "pid.h"

void state_motor_test_wake(struct StateMachine *state_machine)
{
	printf("motor test wake\n");
	*(motor_R.pwm_ccr) = MOTOR_PWM_REGISTER_PERIOD / 4;
}

void state_motor_test_run(struct StateMachine *state_machine)
{
	printf("motor test running\n");
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
///////////////////////
State speed asserv test
///////////////////////
*/

struct PidSettings speed_test_pid_settings = {.kp = 60, .ki = 0.1, .kd = 0.1, .iMax = 500, .iMin = 0};
struct PidRuntime speed_test_pid_runtime = {};


void state_motor_speed_control_test_wake(struct StateMachine *state_machine)
{
	printf("motor speed test wake\n");
	*(motor_R.pwm_ccr) = MOTOR_PWM_REGISTER_PERIOD / 4;
}

void state_motor_speed_control_test_run(struct StateMachine *state_machine)
{
	printf("motor speed test running\n");

	float drive_value = PID_Run(&speed_test_pid_runtime, &speed_test_pid_settings, encoder_R.total_count_delta, 4);
	if (drive_value < 0) drive_value = 0;

	*(motor_R.pwm_ccr) = (uint32_t)drive_value;
}

void state_motor_speed_control_test_stop(struct StateMachine *state_machine)
{
	printf("motor speed test end\n");
}

struct State MOVEMENT_STATE_MOTOR_SPEED_CONTROL_TEST =
{
	.wake = state_motor_speed_control_test_wake,
	.run = state_motor_speed_control_test_run,
	.stop = state_motor_speed_control_test_stop
};




/*
//////////////////////
Statemachine main loop
//////////////////////
*/

struct StateMachine movement_statemachine = {.currentState = 0, .scheduledSwitch = 0};

void movement_statemachine_switch(struct State *state)
{
	SM_Switch(&movement_statemachine, state);
}

void movement_statemachine_update()
{
	SM_Run(&movement_statemachine);
}