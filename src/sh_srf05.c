#include "sh_srf05.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
 
#include <serial/cprintf/cprintf.h>
#include <timer/timer.h>

#include "utils.h"

static srf05_t * dev;

void sh_srf05_set_dev(srf05_t * pdev) {
	dev = pdev;
}

void srf05_disp(void * p) {
	int i;

	cprintf("o ");

	for (i = 0 ; i < 5 ; i++) {
		cprintf("%d ", srf05_read_mm(dev,i));
	}
	cprintf("\r\n");
}

int sh_srf05(int argc, char ** argv) {
	static int streaming = 0;
	static int timer_id = -1;
	uint16_t period;
	uint16_t it_max = 0;
	int ok = 0;

	// o <PERIOD> <IT_MAX>

	if (argc == 1) {
		if (streaming) {
			streaming = 0;
			timer_stop_cb(timer_id);
		}
		else {
			srf05_disp(NULL);
		}
	}
	else {
		if (timer_id == -1)
			timer_id = timer_add_cb(srf05_disp,NULL);

		streaming = 1;
		period = read_uint16(argv[1], &ok);

		timer_start_cb(timer_id, period, it_max);
	}

	return 0;
}
