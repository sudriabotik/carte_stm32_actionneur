# ifndef __MOVEMENT_STATEMACHINE_H
# define __MOVEMENT_STATEMACHINE_H

# include <stdio.h>

# include "state_machine.h"

extern struct State MOVEMENT_STATE_MOTOR_TEST;


void movement_statemachine_switch(struct State *state);
void movement_statemachine_update();

# endif // __MOVEMENT_STATEMACHINE_H