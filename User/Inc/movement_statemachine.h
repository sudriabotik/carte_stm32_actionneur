# ifndef __MOVEMENT_STATEMACHINE_H
# define __MOVEMENT_STATEMACHINE_H

# include "state_machine.h"

/**
 * @file movement_statemachine.h
 * @brief The state machine responsible for controlling the motors to make the robot move.
 */



/**
 * @brief function to be called at regular intervals to run the statemachine logic.
 */
void movement_statemachine_update();


// public functions to interact with the statemachine

/**
 * @brief Checks if the state machine is currently executing a movement.
 * @return 0 if free, 1 if busy.
 */
int movement_statemachine_busy();

/**
 * @brief Uses a trapezoidal command to move in a straight line, then stops.
 * @param distance the distance in mm, can be negative
 * @param speed the targeted top speed, in percentage
 * @param ramp_dist for how many mm at start/end the speed ramps up/down
 */
void movement_statemachine_move_line(float distance, float speed, float ramp_dist);

# endif // __MOVEMENT_STATEMACHINE_H