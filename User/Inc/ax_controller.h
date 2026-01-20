# ifndef __AX_CONTROLLER_H
# define __AX_CONTROLLER_H

#include "stm32g431xx.h"

/**
 * @file ax_controller.h
 * 
 * @brief Library to control AX servomotors
 */



/*
Some links that might be useful

AX-12 control table and other informations :
https://emanual.robotis.com/docs/en/dxl/ax/ax-12a/#

Dynamixel communication protoc 1.0 :
https://emanual.robotis.com/docs/en/dxl/protocol1/
*/

# define AX_UART &huart3

# define AX_DIR_GPIO GPIOC
# define AX_DIR_GPIO_PIN GPIO_PIN_6


/**
 * @brief Enum containing addresses suitable for 1 byte read/write.
 */
enum AX_ADDR_1B_RW
{
	ID = 3,
	BAUD_RATE = 4,
	RETURN_DELAY = 5,
	TEMP_LIMIT = 11,
	MIN_VOLTAGE = 12,
	MAX_VOLTAGE = 13,
	STATUS_RETURN = 16,
	ALARM_LED = 17,
	SHUTDOWN = 18,
	TORQUE_ENABLE = 24,
	LED = 25,
	CW_COMPLIANCE_MARGIN = 26,
	CCW_COMPLIANCE_MARGIN = 27,
	CW_COMPLIANCE_SLOPE = 28,
	CCW_COMPLIANCE_SLOPE = 29,
	EEPROM_LOCK = 47,
};


/**
 * @brief Enum containing addresses suitable for 1 byte read.
 */
enum AX_ADDR_1B_RO
{
	FIRMWARE_VER = 2,
	PRESENT_VOLTAGE = 42,
	PRESENT_TEMPERATURE = 43,
	REGISTERED = 44,
	MOVING = 46,
};


/**
 * @brief Enum containing addresses suitable for 1 byte double read/write.
 * 
 * @note It is used to write two adjacent 8-bits fields at a time, generally min-max values.
 */
enum AX_ADDR_1B_RW_DBL
{
	MIN_MAX_VOLTAGE = 12,
	CW_CCW_COMPLIANCE_MARGIN = 26,
	CW_CCW_COMPLIANCE_SLOPE = 28,
};


/**
 * @brief Enum containing addresses suitable for 2 bytes read/write.
 */
enum AX_ADDR_2B_RW
{
	CW_ANGLE_LIMIT = 6,
	CCW_ANGLE_LIMIT = 8,
	MAX_TORQUE_INIT = 14,
	GOAL_POSITION = 30,
	MOVING_SPEED = 32,
	MAX_TORQUE = 34,
	PUNCH = 48,
};


/**
 * @brief Enum containing addresses suitable for 2 bytes read.
 */
enum AX_ADDR_2B_RO
{
	MODEL_NUM = 0,
	PRESENT_POSITION = 36,
	PRESENT_SPEED = 38,
	PRESENT_LOAD = 40,
};


/**
 * @brief Enum containing addresses suitable for 2 bytes double read/write.
 * 
 * @note It is used to write two adjacent 16-bits fields at a time, generally min-max values.
 */
enum AX_ADDR_2B_RW_DBL
{
	CW_CCW_ANGLE_LIMIT = 6,
	GOAL_POSITION_MOVING_SPEED = 30,
};


void AX_send_write_1B_RW(uint8_t id, enum AX_ADDR_1B_RW address, uint8_t data);
void AX_send_write_1B_RW_DBL(uint8_t id, enum AX_ADDR_1B_RW_DBL address, uint8_t data1, uint8_t data2);


void AX_send_write_2B_RW(uint8_t id, enum AX_ADDR_1B_RW address, uint16_t data);
void AX_send_write_2B_RW_DBL(uint8_t id, enum AX_ADDR_1B_RW_DBL address, uint16_t data1, uint16_t data2);


# endif