# ifndef __STATE_MACHINE_H
# define __STATE_MACHINE_H

# include <stdint.h>

struct State;
struct StateMachine;


typedef void (*StateCall)(struct StateMachine *);



struct State
{
	StateCall wake;
	StateCall stop;

	StateCall run;
};


struct StateMachine
{
	struct State* currentState;
	struct State* scheduledSwitch;
};



struct StateMachine SM_New();


int SM_Run(struct StateMachine *machine);


int SM_Switch(struct StateMachine *machine, struct State *newState);


# endif // __STATE_MACHINE_H