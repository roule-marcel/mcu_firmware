#include "sh_qei.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
 
#include <serial/cprintf/cprintf.h>
#include <timer/timer.h>
#include <pwm/pwm.h>

#include "utils.h"

static qei_t * qei_l;
static qei_t * qei_r;

static pwm_t * pwm_l;
static pwm_t * pwm_r;

void sh_qei_set_dev(qei_t * left, qei_t * right) {
	qei_l = left;
	qei_r = right;
}

void sh_qei_set_pwm(pwm_t * left, pwm_t * right) {
	pwm_l = left;
	pwm_r = right;
}

char s[64];
void disp(void * p) {
	sprintf(s, "%ld %ld", -qei_read_steps(qei_l), qei_read_steps(qei_r));
	cprintf("%s\r\n", s);
}

int sh_qei(int argc, char ** argv) {
	static int streaming = 0;
	static int timer_id = -1;
	uint16_t period;
	uint16_t it_max = 0;
	int ok = 0;

	// e <PERIOD> <IT_MAX> <LEFT RIGHT>

	if (argc == 1) {
		if (streaming) {
			streaming = 0;
			timer_stop_cb(timer_id);
		}
		else {
			disp(NULL);
		}
	}
	else {
		if (timer_id == -1)
			timer_id = timer_add_cb(disp,NULL);

		streaming = 1;
		period = read_uint16(argv[1], &ok);

		if (argc > 2) {
			it_max = read_uint16(argv[2], &ok);

			if (argc > 4) {
				uint16_t left;
				uint16_t right;
				left = read_uint16(argv[3], &ok);
				right = read_uint16(argv[4], &ok);
				cprintf("%d %d\r\n",left, right);

				pwm_set_duty(pwm_l, (float)left/100);
				pwm_set_duty(pwm_r, (float)right/100);
			}
		}

		timer_start_cb(timer_id, period, it_max);
	}

	return 0;
}
