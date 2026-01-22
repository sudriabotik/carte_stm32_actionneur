# ifndef __MOVEMENT_STATEMACHINE_H
# define __MOVEMENT_STATEMACHINE_H

# define MOVEMENT_STATEMACHINE_TEST


/**
 * @file movement_statemachine.h
 * @brief The state machine responsible for controlling the motors to make the robot move.
 */



/**
 * @brief function to be called at regular intervals to run the statemachine logic.
 * 
 * @param delta_time_ms the time between each call, in ms.
 */
void MSM_update(float delta_time_ms);



// public functions to interact with the statemachine

/**
 * @brief Checks if the state machine is currently executing a movement.
 * @return 0 if free, 1 if busy.
 */
int MSM_busy();


/**
 * @brief Holds the robot in place using PIDs.
 */
int MSM_begin_hold();

/**
 * @brief Uses a trapezoidal command to move in a straight line, then stops.
 * @param distance the distance in mm, can be negative
 * @param speed the targeted top speed, in mm/s
 * @param acceleration the acceleration in mm/s^2
 */
int MSM_begin_translation(float distance, float speed, float acceleration);



# ifdef MOVEMENT_STATEMACHINE_TEST

/**
 * @brief 5s of 50% pwm forward for both motors.
 */
void MSM_test_motors();

# endif

# endif // __MOVEMENT_STATEMACHINE_H