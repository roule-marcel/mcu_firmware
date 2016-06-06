#include "uart.h"

#include <serial/uart/uart_regs.h>
#include <hardware/omsp_system.h>

char uart_buffer[_UART_RX_BUFFER_SIZE];
volatile int uart_rx_read_ptr = 0;
volatile int uart_rx_write_ptr = 0;

//--------------------------------------------------//
//        UART RX interrupt service routine         //
//         (receive a byte from the UART)           //
//--------------------------------------------------//
//volatile char rxdata;

wakeup interrupt (UART_RX_VECTOR) INT_uart_rx(void) {
	// Read the received data
	uart_buffer[uart_rx_write_ptr] = UART_RXD;
	// Increment the buffer pointer
	if (uart_rx_write_ptr < _UART_RX_BUFFER_SIZE-1) uart_rx_write_ptr++;
	else uart_rx_write_ptr = 0;
	// Erase last value if buffer is full
	if (uart_rx_write_ptr == uart_rx_read_ptr) uart_rx_read_ptr++;

	// Clear the receive pending flag
	UART_STAT = UART_RX_PND;

	// Exit the low power mode
	LPM0_EXIT;
}

// UART Peripheral initialization
void uart_init(uint32_t baud) {
    UART_BAUD = (CPU_FREQ_HZ/115200)-1;
    UART_CTL  = UART_EN | UART_IEN_RX;
}

int uart_available() {
	if (uart_rx_write_ptr < uart_rx_read_ptr)
		return _UART_RX_BUFFER_SIZE-uart_rx_write_ptr-uart_rx_read_ptr;
	else
		return uart_rx_write_ptr-uart_rx_read_ptr;
}

char uart_read() {
	char ret = uart_buffer[uart_rx_read_ptr];

	if (uart_rx_read_ptr == uart_rx_write_ptr)
		return 0;
	else {
		if (uart_rx_read_ptr < _UART_RX_BUFFER_SIZE-1) uart_rx_read_ptr++;
		else uart_rx_read_ptr = 0;
		return ret;
	}
}
