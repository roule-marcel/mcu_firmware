#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <serial/cprintf/cprintf.h>
#include <serial/uart/uart.h>

#include <hardware/omsp_system.h>
#include <shell/shell.h>

#include <pwm/pwm.h>

#include "sh_reg.h"
#include "sh_pwm.h"

//interrupt (TIMERA1_VECTOR) /*enablenested*/ INT_Timer_overflow(void)
//{
//	static uint8_t blink = 0;
//	static uint16_t cnt = 0;
//
//	if (cnt > 30)
//	{
//		// I'm pretty sure this is self-explanatory
//  		P3OUT = 0xFF * blink;
//  		blink = !blink;
//		cnt = 0;
//	}
//	cnt++;
//
//	// Clear Interrupt
//	TACTL &= ~TAIFG;
//}

//--------------------------------------------------//
// Main function with init an an endless loop that  //
// is synced with the interrupts trough the         //
// lowpower mode.                                   //
//--------------------------------------------------//
int main(void) {
    int reading = 0;
    int pos = 0;
    char buf[40];
    int led = 0;

	pwm_t pwm_0;
	pwm_t pwm_1;

    WDTCTL = WDTPW | WDTHOLD;           // Init watchdog timer

//	// Clock source SMCLK (cause why not?) ; Input devider /8 ; Up mode (count from 0 to TACCR0) ; Enable Interrupts
//	TACTL |= TASSEL1 | ID1 | ID0 | MC0 | TAIE;
//	TACCR0 = 50000;

    P3DIR  = 0xff;
    P3OUT  = 0xff;                      // Light LED during init

	P1DIR = 0x00;

	uart_init(115200);
	shell_init();

//	while ((P1IN & 0x01) == 0);

    P3OUT  = 0x00;                      // Switch off LED

    cprintf("\r\n====== openMSP430 in action ======\r\n");   //say hello
    cprintf("\r\nSimple Line Editor Ready\r\n");
	cprintf("\r\nBAUD=%d\r\n", (CPU_FREQ_HZ/115200)-1);

	cprintf("\r\ntest=%c\r\n", 'x');

	shell_add('w', sh_reg_write);
	shell_add('r', sh_reg_read);
	shell_add('p', sh_pwm);

	pwm_init(&pwm_0, 0x0180, 20000, 2);
	pwm_enable(&pwm_0);

	pwm_init(&pwm_1, 0x0188, 20000, 2);
	pwm_enable(&pwm_1);

	set_pwm_dev(&pwm_0, &pwm_1);

    eint();                             // Enable interrupts

    while (1) {                         //main loop, never ends...
        cprintf("> ");                   //show prompt

        reading = 1;
        while (reading) {               //loop and read characters

            LPM0;                       //sync, wakeup by irq

		    led++;                      // Some lighting...
		    if (led==9) {
		      led = 0;
		    }
		    P3OUT = (0x01 << led);

			if (uart_available()) {
				char c = uart_read();
				switch (c) {
					//process RETURN key
					case '\r':
						//case '\n':
						cprintf("\r\n");    //finish line
						buf[pos++] = 0;     //to use cprintf...
						cprintf(":%s\r\n", buf);
						reading = 0;        //exit read loop
						pos = 0;            //reset buffer
						break;
						//backspace
					case '\b':
						if (pos > 0) {      //is there a char to delete?
							pos--;          //remove it in buffer
							cprintf("\b \b");
						}
						break;
						//other characters
					default:
						//only store characters if buffer has space
						if (pos < sizeof(buf)) {
							cprintf("%c", c);     //echo
							buf[pos++] = c; //store
						}
				}
			}
		}

		shell_exec(buf[0], buf);
	}
}
