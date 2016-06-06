#include "sh_pwm.h"

#include <stdint.h>
 
#include <serial/cprintf/cprintf.h>

#include "utils.h"

pwm_t * pwm_l;
pwm_t * pwm_r;

void set_pwm_dev(pwm_t * left, pwm_t * right) {
	pwm_l = left;
	pwm_r = right;
}

int sh_pwm(char * buf) {
	uint16_t left;
	uint16_t right;
	int ok;

	if (buf[1] != ' ' || buf[2] == 0) {
		cprintf("correct usage:\r\n");
		cprintf("\t%c LEFT RIGHT\r\n", buf[0]);
		return -1;
	}

	buf = &buf[2];
	buf = read_uint16(&left, buf, &ok);
	if (ok == 0) {
		cprintf("Something went terribly wrong\r\n");
		return -1;
	}
	buf = read_uint16(&right, buf, &ok);
	if (ok == 0) {
		cprintf("Something went terribly wrong\r\n");
		return -1;
	}

	pwm_set_duty(pwm_l, (float)left/100);
	pwm_set_duty(pwm_r, (float)right/100);

	return 0;
}
