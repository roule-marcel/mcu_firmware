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

#include "speed.h"

#include "sh_reg.h"
#include "sh_pwm.h"
#include "sh_qei.h"
#include "sh_speed.h"
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

int interactive = 0;
int sh_interactive(int argc, char ** argv) {
	interactive = !interactive;
	
	return 0;
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

	pwm_t pwm_l;
	pwm_t pwm_r;

	qei_t qei_l;
	qei_t qei_r;

	pid_t pid_l;
	pid_t pid_r;

    WDTCTL = WDTPW | WDTHOLD;           // Init watchdog timer

    P3DIR  = 0xff;
//    P3OUT  = 0xff;                      // Light LED during init

	P1DIR = 0x00;

	uart_init(115200);
	shell_init();

    P3OUT  = 0x00;                      // Switch off LED

	cprintf("\r\n====== Marcel MCU ======\r\n");   //say hello

	shell_add('i', sh_interactive, "interactive mode");
	shell_add('w', sh_reg_write, "write");
	shell_add('r', sh_reg_read, "read");
	shell_add('p', sh_pwm, "pwm");
	shell_add('e', sh_qei, "encoder");
	shell_add('s', sh_speed, "speed controller");
	shell_add('c', sh_speed_config, "speed configuration");
	shell_add('b', sh_bootloader, "bootloader");

	sh_help(0, NULL);

	pwm_init(&pwm_l, 0x0180, 20000, 2);
	pwm_enable(&pwm_l);

	pwm_init(&pwm_r, 0x0188, 20000, 2);
	pwm_enable(&pwm_r);

	sh_pwm_set_dev(&pwm_l, &pwm_r);

	qei_init(&qei_l, 0x0198);
	qei_init(&qei_r, 0x019C);
	sh_qei_set_dev(&qei_l, &qei_r);
	sh_qei_set_pwm(&pwm_l, &pwm_r);

	timer_init();
//	id1 = timer_add_cb(qei_sim,0);
//	timer_start_cb(id1, 1, 0);
	id1 = timer_add_cb(blink,(void*)0xFF);
	timer_start_cb(id1, 1000, 0);

	speed_init(&pid_l, 1, 0, 0);
	speed_init(&pid_r, 1, 0, 0);
	sh_pid_set_dev(&pid_l, &pid_r);

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
					buf[pos] = 0;
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
						if (interactive)
							cprintf("%c", c);     //echo
						buf[pos++] = c; //store
					}
			}
		}
	}
}
