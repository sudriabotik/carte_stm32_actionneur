# ifndef __UART_MESSENGER_H
# define __UART_MESSENGER_H


# include "stm32g431xx.h"
# include "stm32g4xx_hal.h"
# include "usart.h"



void uart_messenger_init();

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);


# endif