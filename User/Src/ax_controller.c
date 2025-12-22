# include "ax_controller.h"


# define AX_UART &hlpuart1 // temp


void ax_init() {}

// byte per byte :
// <header> <header> <id> <length 4> <instruction write> <address 30 position goal> <position> <position> <checksum>
uint8_t ax_write_position_msg[] = {0xFF, 0xFF, 0x00, 0x04, 0x03, 0x1E, 0x00, 0x00, 0x00};

/**
 * @brief Write to the "Goal Position" table entry of AX ID
 * 
 * @note Position should be between 0 and 1023
 */
void ax_write_position(uint8_t id, uint16_t position)
{
	// write id
	ax_write_position_msg[2] = id;

	// write position
	ax_write_position_msg[6] = (uint8_t)(position >> 8) & 0xFF;
	ax_write_position_msg[7] = (uint8_t)(position) & 0xFF;

	// sets checksum
	ax_write_position_msg[8] = ~ (ax_write_position_msg[2] + ax_write_position_msg[3] + ax_write_position_msg[4] + ax_write_position_msg[5] + ax_write_position_msg[6] + ax_write_position_msg[7]);

	// sends the packet
	HAL_UART_Transmit_IT(AX_UART, ax_write_position_msg, 9);
}