# ifndef __PUTCHAR_H
# define __PUTCHAR_H

# define PUTCHAR_PROTOTYPE int __io_putchar(int ch)

UART_HandleTypeDef *com_uart;

void printf_redir_init(UART_HandleTypeDef *uart);

PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(com_uart, (uint8_t*)&ch, 1, 0xFFFF);
	return ch;
}

# endif