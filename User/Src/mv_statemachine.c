#include "mv_statemachine.h"

#include <stdio.h>



struct MvStateMachine MSM_init()
{
	struct MvStateMachine machine =
	{
		.index = 0,
		.construction_index = 0,
		.load_construction_queue = 0,
	};

	for (uint_t i = 0; i <= MV_STATEMACHINE_LENGTH; i++) machine.state_queue[i] = 0;

	return machine;
}

int_t MSM_reset_construction(struct MvStateMachine *machine)
{
	machine->construction_index = 0;
	return 0;
}

int_t MSM_enqueue_state(struct MvStateMachine *machine, struct MvState *state, struct MvStateEnv env)
{
	if (MSM_is_construction_full(machine)) return -1;

	machine->construction_state_queue[machine->construction_index] = state;
	machine->construction_envs_queue[machine->construction_index] = env;
	machine->construction_index ++;

	return 0;
}

int_t MSM_ready_construction(struct MvStateMachine *machine)
{
	machine->load_construction_queue = 1;
	return 0;
}

uint_t MSM_get_construction_index(struct MvStateMachine *machine)
{
	return machine->construction_index;
}

int_t MSM_is_construction_full(struct MvStateMachine *machine)
{
	return machine->construction_index >= MV_STATEMACHINE_LENGTH;
}

int MSM_is_busy(struct MvStateMachine *machine)
{
	return machine->state_queue[machine->index] == 0;
}

int MSM_update(struct MvStateMachine *machine, float delta_time)
{
	struct MvState *current_state = machine->state_queue[machine->index];
	struct MvStateEnv *current_env = &machine->envs_queue[machine->index];

	// tests if a new state sequence is ready to be loaded in
	if (machine->load_construction_queue)
	{
		machine->load_construction_queue = 0;
	
		// exits the current state
		if (current_state != 0) if (current_state->stop != 0) current_state->stop(machine, current_env, delta_time);
		// loads the construction state queue as the current state queue
		uint_t i = 0;
		for (i = 0; i < machine->construction_index; i++)
		{
			machine->state_queue[i] = machine->construction_state_queue[i];
		}
		for (; i < MV_STATEMACHINE_LENGTH; i++)
		{
			machine->state_queue[i] = 0;
		}
		machine->index = 0;
		machine->state_finished = 0;

		// updates the current state and env
		current_state = machine->state_queue[machine->index];
		current_env = &machine->envs_queue[machine->index];


		// wakes up the new state
		if (current_state != 0) if (current_state->stop != 0) current_state->wake(machine, current_env, delta_time);
	}

	// test if the currently active state has finished
	if (machine->state_finished == 1)
	{
		// exits the current state
		if (current_state != 0) if (current_state->stop != 0) current_state->stop(machine, current_env, delta_time);

		if (machine->index < MV_STATEMACHINE_LENGTH)
		{
			machine->index ++;

			// updates the current state and env
			current_state = machine->state_queue[machine->index];
			current_env = &machine->envs_queue[machine->index];

			// wakes up the current state
			if (current_state != 0) if (current_state->stop != 0) current_state->wake(machine, current_env, delta_time);

			machine->state_finished = 0;
		}

	}

	if (current_state != 0) current_state->run(machine, current_env, delta_time);

	return 0;
}

int_t MSM_set_state_finished(struct MvStateMachine *machine)
{
	machine->state_finished = 1;
	return 0;
}