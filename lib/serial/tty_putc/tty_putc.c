#include "tty_putc.h"

#include <serial/uart/uart_regs.h>

//#include "hardware.h"

//--------------------------------------------------//
//                 tty_putc function                //
//            (Send a byte to the UART)             //
//--------------------------------------------------//
int tty_putc (int txdata) {

  // Wait until the TX buffer is not full
  while (UART_STAT & UART_TX_FULL);

  // Write the output character
  UART_TXD = txdata;

  return 0;
}
