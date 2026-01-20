# ifndef __AX_CONTROLLER_H
# define __AX_CONTROLLER_H

#include "stm32g431xx.h"


/*
Some links that might be useful :
https://emanual.robotis.com/docs/en/dxl/ax/ax-12a/#
https://emanual.robotis.com/docs/en/dxl/protocol1/
*/

# define AX_UART &huart3


void ax_write_position(uint8_t id, uint16_t position);


# endif