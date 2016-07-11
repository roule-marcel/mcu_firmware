#include "sh_pwm.h"

#include <stdlib.h>
#include <stdint.h>
 
#include <serial/cprintf/cprintf.h>
#include <timer/timer.h>

#include "utils.h"

pwm_t * pwm_l;
pwm_t * pwm_r;

void pwm_timeout_cb(void * p) {
	pwm_set_duty(pwm_l, 0.5);
	pwm_set_duty(pwm_r, 0.5);

	cprintf("Stop\r\n");
}

void sh_pwm_set_dev(pwm_t * left, pwm_t * right) {
	pwm_l = left;
	pwm_r = right;
}

int sh_pwm(int argc, char ** argv) {
	uint16_t timeout = 0;
	uint16_t left;
	uint16_t right;
	static int timer_id = -1;
	int ok;

	if (timer_id == -1)
		timer_id = timer_add_cb(pwm_timeout_cb,NULL);

	if (argc < 3) {
		cprintf("error: missing argument\r\n");
		cprintf("correct usage:\r\n");
		cprintf("\t%s LEFT RIGHT <TIMEOUT>\r\n", argv[0]);

		return -1;
	}

	if (argc > 3) {
		timeout = read_uint16(argv[3], &ok);
		timer_start_cb(timer_id, timeout, 1);
	}

	left = read_uint16(argv[1], &ok);
	right = read_uint16(argv[2], &ok);
	cprintf("%d %d\r\n",left, right);

	pwm_set_duty(pwm_l, 1-(float)left/100);
	pwm_set_duty(pwm_r, (float)right/100);

	return 0;
}
