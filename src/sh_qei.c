#include "sh_qei.h"

#include <stdlib.h>
#include <stdint.h>
 
#include <serial/cprintf/cprintf.h>
#include <timer/timer.h>

#include "utils.h"

qei_t * qei_l;
qei_t * qei_r;

void sh_qei_set_dev(qei_t * left, qei_t * right) {
	qei_l = left;
	qei_r = right;
}

void disp(void * p) {
	cprintf("%w %w %w %w\r\n", 
			(uint16_t)(*(qei_l->reg) >> 16),
			(uint16_t)(*(qei_l->reg)),
			(uint16_t)(*(qei_r->reg) >> 16),
			(uint16_t)(*(qei_r->reg)));
}

int sh_qei(int argc, char ** argv) {
	static int streaming = 0;
	static int timer_id = -1;
	uint16_t period;
	uint16_t it_max = 0;
	int ok = 0;

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
		}

		timer_start_cb(timer_id, period, it_max);
	}

	return 0;
}
