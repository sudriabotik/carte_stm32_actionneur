#ifndef __MV_STATEMACHINE_STATES_H
#define __MV_STATEMACHINE_STATES_H

/**
  * @file mv_statemachine_states.h
  *
  * @brief contains the definition of every MvState.
  * There are also helper functions to generate a correct environment for each state.
  */

#include "mv_statemachine.h"


extern struct MvStateMachine MV_STATEMACHINE;

extern struct MvState MV_STATE_HOLD;
struct MvStateEnv genenv_mv_state_hold();

extern struct MvState MV_STATE_TRANSLATION;
struct MvStateEnv genenv_mv_state_translation(float acceleration, float speed, float distance);

extern struct MvState MV_STATE_ROTATION;
struct MvStateEnv genenv_mv_state_rotation(float acceleration, float speed, float distance);

extern struct MvState MV_STATE_RECALIBRATION;
struct MvStateEnv genenv_mv_state_recalibration(float acceleration, float speed, enum Facing facing);



#endif
