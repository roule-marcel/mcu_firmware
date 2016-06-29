#include "sh_pwm.h"

#include <stdint.h>
 
#include <serial/cprintf/cprintf.h>

#include "utils.h"

pwm_t * pwm_l;
pwm_t * pwm_r;

void sh_pwm_set_dev(pwm_t * left, pwm_t * right) {
	pwm_l = left;
	pwm_r = right;
}

int sh_pwm(int argc, char ** argv) {
	uint16_t left;
	uint16_t right;
	int ok;

	if (argc < 3) {
		cprintf("error: missing argument\r\n");
		cprintf("correct usage:\r\n");
		cprintf("\t%s LEFT RIGHT\r\n", argv[0]);

		return -1;
	}

	left = read_uint16(argv[1], &ok);
	right = read_uint16(argv[2], &ok);
	cprintf("%d %d\r\n",left, right);

	pwm_set_duty(pwm_l, (float)left/100);
	pwm_set_duty(pwm_r, (float)right/100);

	return 0;
}
