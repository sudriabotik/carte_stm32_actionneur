#include "mv_statemachine.h"

#include <stdio.h>



// Crée et retourne une machine à états vide avec toutes les files initialisées à zéro
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

// Remet la file de construction à zéro pour pouvoir en préparer une nouvelle séquence
int_t MSM_reset_construction(struct MvStateMachine *machine)
{
	machine->construction_index = 0;
	return 0;
}

// Ajoute un état et ses paramètres à la fin de la file de construction
int_t MSM_enqueue_state(struct MvStateMachine *machine, struct MvState *state, struct MvStateEnv env)
{
	if (MSM_is_construction_full(machine)) return -1;

	machine->construction_state_queue[machine->construction_index] = state;
	machine->construction_envs_queue[machine->construction_index] = env;
	machine->construction_index ++;

	return 0;
}

// Marque la file de construction comme prête : elle sera chargée au prochain appel à MSM_update()
int_t MSM_ready_construction(struct MvStateMachine *machine)
{
	machine->load_construction_queue = 1;
	return 0;
}

// Retourne le nombre d'états actuellement dans la file de construction
uint_t MSM_get_construction_index(struct MvStateMachine *machine)
{
	return machine->construction_index;
}

// Retourne 1 si la file de construction a atteint sa capacité maximale (MV_STATEMACHINE_LENGTH)
int_t MSM_is_construction_full(struct MvStateMachine *machine)
{
	return machine->construction_index >= MV_STATEMACHINE_LENGTH;
}

// Retourne 1 si la machine exécute encore un état (file active non terminée)
int MSM_is_busy(struct MvStateMachine *machine)
{
	return machine->state_queue[machine->index] != 0;
}

// Coeur de la machine : charge la file de construction si prête, avance à l'état suivant si le courant est terminé, puis exécute run() de l'état actif
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
			machine->envs_queue[i] = machine->construction_envs_queue[i];
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

		printf("loaded new state queue\n");
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

// Signale que l'état en cours est terminé ; MSM_update() passera au suivant lors du prochain appel
int_t MSM_set_state_finished(struct MvStateMachine *machine)
{
	machine->state_finished = 1;
	return 0;
}