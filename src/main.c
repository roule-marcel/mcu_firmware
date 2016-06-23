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
#include "sh_boot.h"

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

    P3DIR  = 0xff;
//    P3OUT  = 0xff;                      // Light LED during init

	P1DIR = 0x00;

	uart_init(9600);
	shell_init();

    P3OUT  = 0x00;                      // Switch off LED

	cprintf("\r\n====== Marcel MCU ======\r\n");   //say hello

	shell_add('w', sh_reg_write);
	shell_add('r', sh_reg_read);
	shell_add('p', sh_pwm);
	shell_add('b', sh_bootloader);

	pwm_init(&pwm_0, 0x0180, 20000, 2);
	pwm_enable(&pwm_0);

	pwm_init(&pwm_1, 0x0188, 20000, 2);
	pwm_enable(&pwm_1);

	set_pwm_dev(&pwm_0, &pwm_1);

	P3OUT = 0x80;

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
