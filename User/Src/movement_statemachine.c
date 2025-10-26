# include "movement_statemachine.h"

void state_motor_test_wake(struct StateMachine *state_machine)
{
	printf("motor test wake\n");
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