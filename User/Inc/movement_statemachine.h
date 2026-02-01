# ifndef __MOVEMENT_STATEMACHINE_H
# define __MOVEMENT_STATEMACHINE_H

# define MOVEMENT_STATEMACHINE_TEST


/**
 * @file movement_statemachine.h
 * @brief The state machine responsible for controlling the motors to make the robot move.
 */



enum Facing
{
	POSITIVE_X,
	POSITIVE_Y,
	NEGATIVE_X,
	NEGATIVE_Y
};



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


/**
 * @brief Recalibrate the position using a wall
 *
 * @param motor_rps the rps to run the motor at. Can be negative.
 * @param stop_threshold the speed integral threshold to stop the motors at.
 * @param power_limit the maximum power, as a percentage, to be used from the motors.
 * @param facing where the front of the robot should be facing.
 * 
 * @note To calibrate with the back of the robot, give a negative speed.
 * The robot will move in the direction given as facing.
 */
int MSM_begin_recalibration(float motor_rps, float stop_threshold, float power_limit, enum Facing facing);


# ifdef MOVEMENT_STATEMACHINE_TEST

/**
 * @brief 5s of 50% pwm forward for both motors.
 */
void MSM_test_motors();

# endif

# endif // __MOVEMENT_STATEMACHINE_H
