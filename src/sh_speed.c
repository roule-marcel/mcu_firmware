#include "sh_speed.h"

#include <stdlib.h>

#include <serial/cprintf/cprintf.h>
#include <timer/timer.h>

#include "utils.h"

speed_t * speed_l;
speed_t * speed_r;
srf05_t * srf05;

int timer_id = -1;

void speed_timeout_cb(void * p) {
//	speed_stop(speed_l);
//	speed_stop(speed_r);
	speed_setPoint(speed_l, 0);
	speed_setPoint(speed_r, 0);
	
	cprintf("Stop\r\n");
}

void sh_speed_init() {
	if (timer_id == -1)
		timer_id = timer_add_cb(speed_timeout_cb,NULL);

	speed_start(speed_l);
	speed_start(speed_r);
}

void sh_speed_set_dev(speed_t * left, speed_t * right) {
	speed_l = left;
	speed_r = right;
}

void sh_speed_set_srf05(srf05_t * dev) {
	srf05 = dev;
}

int sh_speed(int argc, char ** argv) {
	uint16_t timeout = 0;
	int left;
	int right;
	int ok;


	if (argc < 3) {
		cprintf("correct usage:\r\n");
		cprintf("\t%c [o] LEFT RIGHT <TIMEOUT>\r\n", argv[0]);
		return -1;
	}

	if (argc > 3) {
		timeout = read_uint16(argv[3], &ok);
		timer_start_cb(timer_id, timeout, 1);
	}

	left = read_int16(argv[1], &ok);
	right = read_int16(argv[2], &ok);
	cprintf("%d %d\r\n",left, right);

//	speed_start(speed_l);
//	speed_start(speed_r);

	speed_setPoint(speed_l, -left);
	speed_setPoint(speed_r, right);

	return 0;
}

void sh_speed_arrow_up() {
	speed_setPoint(speed_l, -100);
	speed_setPoint(speed_r, 100);

	timer_start_cb(timer_id, 200, 1);
}

void sh_speed_arrow_down() {
	speed_setPoint(speed_l, 100);
	speed_setPoint(speed_r, -100);

	timer_start_cb(timer_id, 200, 1);
}

void sh_speed_arrow_right() {
	speed_setPoint(speed_l, -60);
	speed_setPoint(speed_r, -60);

	timer_start_cb(timer_id, 200, 1);
}

void sh_speed_arrow_left() {
	speed_setPoint(speed_l, 60);
	speed_setPoint(speed_r, 60);

	timer_start_cb(timer_id, 200, 1);
}

int sh_speed_config(int argc, char ** argv) {
	cprintf("Not implemented Yet\r\n");
	return 0;
}
