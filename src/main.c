#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <serial/cprintf/cprintf.h>
#include <serial/uart/uart.h>

#include <hardware/omsp_system.h>
#include <shell/shell.h>

#include <pwm/pwm.h>
#include <qei/qei.h>
#include <timer/timer.h>

#include "sh_reg.h"
#include "sh_pwm.h"
#include "sh_qei.h"
#include "sh_boot.h"

void blink (void * p) {
	int pattern = (uint16_t)p;

	if (P3OUT & pattern)
		P3OUT &= ~pattern;
	else
		P3OUT |= pattern;
}

void qei_sim (void * p) {
	static int state = 0;

	switch (state) {
		case 0:
			P3OUT = 0;
			break;
		case 1:
			P3OUT = 4;
			break;
		case 2:
			P3OUT = 13;
			break;
		case 3:
			P3OUT = 9;
			break;
		case 4:
			P3OUT = 3;
			break;
		case 5:
			P3OUT = 7;
			break;
		case 6:
			P3OUT = 14;
			break;
		case 7:
			P3OUT = 10;
			break;
		default:
			break;
	}

	if (state < 7) state++;
	else state = 0;
}

//--------------------------------------------------//
// Main function with init an an endless loop that  //
// is synced with the interrupts trough the         //
// lowpower mode.                                   //
//--------------------------------------------------//
int main(void) {
    int pos = 0;
    char buf[40];

	int id1;

	pwm_t pwm_0;
	pwm_t pwm_1;

	qei_t qei_0;
	qei_t qei_1;

    WDTCTL = WDTPW | WDTHOLD;           // Init watchdog timer

    P3DIR  = 0xff;
//    P3OUT  = 0xff;                      // Light LED during init

	P1DIR = 0x00;

	uart_init(9600);
	shell_init();

    P3OUT  = 0x00;                      // Switch off LED

	cprintf("\r\n====== Marcel MCU ======\r\n");   //say hello

	shell_add('w', sh_reg_write, "write");
	shell_add('r', sh_reg_read, "read");
	shell_add('p', sh_pwm, "pwm");
	shell_add('e', sh_qei, "encoder");
	shell_add('b', sh_bootloader, "bootloader");

	sh_help(0, NULL);

	pwm_init(&pwm_0, 0x0180, 20000, 2);
	pwm_enable(&pwm_0);

	pwm_init(&pwm_1, 0x0188, 20000, 2);
	pwm_enable(&pwm_1);

	sh_pwm_set_dev(&pwm_0, &pwm_1);

	qei_init(&qei_0, 0x0198);
	qei_init(&qei_1, 0x019C);
	sh_qei_set_dev(&qei_0, &qei_1);

	timer_init();
	id1 = timer_add_cb(1,qei_sim,0);
	timer_start_cb(id1);

	P3OUT = 0x80;

    eint();                             // Enable interrupts

    while (1) {                         //main loop, never ends...
		if (uart_available()) {
			char c = uart_read();
			switch (c) {
				//process RETURN key
				case '\r':
					//case '\n':
					cprintf("\r\n");    //finish line
					pos = 0;            //reset buffer

					// Call the function
					shell_exec(buf[0], buf);

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
}
