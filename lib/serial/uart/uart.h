#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>

#define _UART_RX_BUFFER_SIZE 64

void uart_init(uint32_t baud);
int uart_available();
char uart_read();

#endif //__UART_H__
