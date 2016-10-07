#include "sh_odometry.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
 
#include <serial/cprintf/cprintf.h>
#include <timer/timer.h>

#include "utils.h"

static odometry_t * dev;

void sh_odometry_set_dev(odometry_t * pdev) {
	dev = pdev;
}

char s[64];
void odometry_disp(void * p) {
	cprintf("z ");
	sprintf(s, "%d %d %d", (int)(dev->position.x*1000), (int)(dev->position.y*1000), (int)(dev->position.theta*1000));
	cprintf("%s\r\n", s);
}

int sh_odometry(int argc, char ** argv) {

	static int streaming = 0;
	static int timer_id = -1;
	uint16_t period;
	uint16_t it_max = 0;
	int ok = 0;

	// z <PERIOD> <IT_MAX>

	if (argc == 1) {
		if (streaming) {
			streaming = 0;
			timer_stop_cb(timer_id);
		}
		else {
			odometry_disp(NULL);
		}
	}
	else {
		if (timer_id == -1)
			timer_id = timer_add_cb(odometry_disp,NULL);

		streaming = 1;
		period = read_uint16(argv[1], &ok);

		timer_start_cb(timer_id, period, it_max);
	}

	return 0;
}
