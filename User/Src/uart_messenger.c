# include "uart_messenger.h"


uint8_t buffer[20];


void uart_messenger_init()
{
	HAL_UART_Receive_IT(&huart3, buffer, 20);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	HAL_UART_Receive_IT(&huart3, buffer, 20);
	HAL_UART_Transmit_IT(&huart3, buffer, 20);
}
