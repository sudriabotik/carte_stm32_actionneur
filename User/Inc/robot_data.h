# ifndef __ROBOT_DATA_H
# define __ROBOT_DATA_H

# include "encoders.h"
# include "motors.h"


/**
 * @brief The diameter of the robot's wheels, in mm.
 */
extern const float ROBOT_WHEEL_DIAMETER;

/**
 * @brief The diameter of the robot's wheels, in mm.
 */
extern const float ROBOT_ENCODER_WHEEL_DIAMETER;

/**
 * @brief The width between each wheel, in mm.
 */
extern const float ROBOT_WHEELBASE_WIDTH;

/**
 * @brief Corrective coefficient for the distance of translation movement
 */
extern const float ROBOT_COEF_TRANSLATION_DISTANCE;

/**
 * @brief Corrective coefficient for the target distance of the right wheel, for a translation movement.
 * The left wheel will be corrected by (1 - coef)
 * 
 * @note If the coefficient is 1.2, the right wheel will move 1.2 * distance while the left wheel will move 0.8 * distance.
 */
extern const float ROBOT_COEF_WHEEL_DIFFERENCE;

/**
 * @brief Corrective coefficient for rotation movement
 */
extern const float ROBOT_COEF_ROTATION;


extern struct Encoder16Handle encoder_R;
extern struct MotorHandle motor_R;

extern struct Encoder16Handle encoder_L;
extern struct MotorHandle motor_L;

/**
 * @brief Initialize the struct handles to the various devices of the robot.
 * 
 * @note Always call this at the start of the program before accessing any of the structs in this file
 * 
 */
void robot_data_init();

# endif // __ROBOT_DATA_H