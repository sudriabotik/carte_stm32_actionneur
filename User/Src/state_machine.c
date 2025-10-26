# include "state_machine.h"

struct StateMachine SM_New()
{
	return (struct StateMachine) {.currentState = 0, .scheduledSwitch = 0};
}


int SM_Run(struct StateMachine *machine)
{
	if (machine == 0) return -1;

	if (machine->scheduledSwitch != 0)
	{
		if (machine->currentState != 0) machine->currentState->stop(machine);
		machine->currentState = machine->scheduledSwitch;
		machine->scheduledSwitch = 0;
		machine->currentState->wake(machine);
	}

	if (machine->currentState != 0) machine->currentState->run(machine);

	return 0;
}


int SM_Switch(struct StateMachine *machine, struct State *newState)
{
	if (machine == 0) return -1;

	machine->scheduledSwitch = newState;

	return 0;
}