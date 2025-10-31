# include "movement_statemachine.h"

# include "motors.h"

void state_motor_test_wake(struct StateMachine *state_machine)
{
	printf("motor test wake\n");
	*(motor_R.pwm_ccr) = MOTOR_PWM_REGISTER_PERIOD / 4;
	//TIM16->CCR1 = MOTOR_PWM_REGISTER_PERIOD / 4;
	HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
}

void state_motor_test_run(struct StateMachine *state_machine)
{
	printf("motor test running\n");
	//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
	//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
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