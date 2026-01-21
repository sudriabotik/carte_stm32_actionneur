# include "state_machine.h"

struct StateMachine SM_New()
{
	return (struct StateMachine) {.currentState = 0, .scheduledSwitch = 0};
}


int SM_Run(struct StateMachine *machine, float delta_time)
{
	if (machine == 0) return -1;

	if (machine->scheduledSwitch != 0)
	{
		if (machine->currentState != 0) machine->currentState->stop(machine, delta_time);
		machine->currentState = machine->scheduledSwitch;
		machine->scheduledSwitch = 0;
		machine->currentState->wake(machine, delta_time);
	}

	if (machine->currentState != 0) machine->currentState->run(machine, delta_time);

	return 0;
}


int SM_Switch(struct StateMachine *machine, struct State *newState)
{
	if (machine == 0) return -1;

	machine->scheduledSwitch = newState;

	return 0;
}