# include "movement_statemachine.h"

# include "motors.h"

void state_motor_test_wake(struct StateMachine *state_machine)
{
	printf("motor test wake\n");
	*motor_R.pwm_ccr = MOTOR_PWM_REGISTER_PERIOD / 2;
	HAL_GPIO_WritePin(GPIOB, 5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, 4, GPIO_PIN_RESET);
}

void state_motor_test_run(struct StateMachine *state_machine)
{
	printf("motor test running\n");
	HAL_GPIO_TogglePin(GPIOB, 5);
	HAL_GPIO_TogglePin(GPIOB, 4);
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