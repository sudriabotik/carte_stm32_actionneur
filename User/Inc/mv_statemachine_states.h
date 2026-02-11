#ifndef __MV_STATEMACHINE_H
#define __MV_STATEMACHINE_H

/**
  * @file mv_statemachine_states.h
  *
  * @brief contains the definition of every MvState.
  * There are also helper functions to generate a correct environment for each state.
  */

#include "mv_statemachine.h"



extern struct MvStateEnv mv_state_hold;
void genenv_mv_state_hold();



#endif
