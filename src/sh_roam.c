#include "sh_roam.h"

#include <stdlib.h>
#include <timer/timer.h>
#include <serial/cprintf/cprintf.h>

#define ST_ROAM_GO_AHEAD 0
#define ST_ROAM_OBSTACLE_FRONT 1
#define ST_ROAM_OBSTACLE_LEFT 2
#define ST_ROAM_OBSTACLE_RIGHT 3
#define ST_ROAM_OBSTACLE_REAR 4

static int roam_state = 0;

static speed_t * speed_l;
static speed_t * speed_r;
static srf05_t * srf05;
static int roaming = 0;
static int roam_timer_id;
static uint16_t srf05_limits[5] = {500, 500, 500, 500, 0};
static uint16_t stop_delay = 0;
static uint16_t rotation_delay = 0;

uint16_t us_read(srf05_t * dev) {
	int i;
	uint16_t ret = 0;
	for (i = 0 ; i < 5 ; i++) {
		if (srf05_read_mm(dev, i) < srf05_limits[i]) {
			ret += (1<<i);
		}
	}
	return ret;
}

void roam_cb (void * p) {
	int i;
	uint16_t us;

//	cprintf("roam_cb\r\n");

	if (roaming) {
		us = us_read(srf05);

		for (i = 0 ; i < 5 ; i++) {
			cprintf("%d ", srf05_read_mm(srf05,i));
		}
		cprintf("\r\n");

		if (roam_state == ST_ROAM_GO_AHEAD) {
			// Every US is above threshold
			// Nothing dangerous
			// Go ahead
			if (us == 0) {
				speed_setPoint(speed_l, 60);
				speed_setPoint(speed_r, -60);
			}
			// Back Left US or Back Right US under threshold
			// Someone probably following the robot
			// Stop a few seconds just in case then go ahead
			else if (us & 0x08) {
//				roam_state = ST_ROAM_OBSTACLE_REAR;
//				speed_setPoint(speed_l, 0);
//				speed_setPoint(speed_r, 0);
//				stop_delay = 20;
//				rotation_delay = 0;

				cprintf("Yo behind me\r\n");
			}
			// Front US under threshold and Front Left US ~ Front Right US
			// There is something in front of the robot
			// Stop a few seconds then take a random angle
	
			// Front US under threshold and Front Left US << Front Right US
			// There is something in front left of the robot
			// Stop a few seconds then turn right
	
			// Front US under threshold and Front Left US >> Front Right US
			// There is something in front right of the robot
			// Stop a few seconds then turn left
			else if (us & 0x2) {
				roam_state = ST_ROAM_OBSTACLE_FRONT;
				speed_setPoint(speed_l, 0);
				speed_setPoint(speed_r, 0);
				stop_delay = 20;
				rotation_delay = 20;
				cprintf("Yo in front\r\n");
			}
			// Front Right US under threshold
			// Theres is something in front right of the robot
			// Stop a few seconds then turn left
			else if (us & 0x1) {
				roam_state = ST_ROAM_OBSTACLE_RIGHT;
				speed_setPoint(speed_l, 0);
				speed_setPoint(speed_r, 0);
				stop_delay = 20;
				rotation_delay = 10;
				cprintf("Yo on the right\r\n");
			}
			// Front Left US under threshold
			// Theres is something in front left of the robot
			// Stop a few seconds then turn right
			else if (us & 0x4) {
				roam_state = ST_ROAM_OBSTACLE_LEFT;
				speed_setPoint(speed_l, 0);
				speed_setPoint(speed_r, 0);
				stop_delay = 20;
				rotation_delay = 10;
				cprintf("Yo on the left\r\n");
			}
	
			else {
				speed_setPoint(speed_l, 0);
				speed_setPoint(speed_r, 0);
	
				cprintf("%d\r\n", us);
			}
		}
		else {
			if (stop_delay) {
				speed_setPoint(speed_l, 0);
				speed_setPoint(speed_r, 0);
				cprintf("stop\r\n");

				stop_delay--;

				// Maybe the obstacle is gone ?
				if (us == 0) {
					stop_delay = 0;
					rotation_delay = 0;
					roam_state = ST_ROAM_GO_AHEAD;
				}
			}
			else {
				if (rotation_delay) {
					if (roam_state == ST_ROAM_OBSTACLE_FRONT) {
						if (srf05_read_mm(srf05, SRF05_FRONT_LEFT) < srf05_read_mm(srf05, SRF05_FRONT_RIGHT)) {
							speed_setPoint(speed_l, 30);
							speed_setPoint(speed_r, 30);
						}
						else {
							speed_setPoint(speed_l, -30);
							speed_setPoint(speed_r, -30);
						}
						cprintf("front:rotation\r\n");
					}
					else if (roam_state == ST_ROAM_OBSTACLE_LEFT) {
						speed_setPoint(speed_l, -30);
						speed_setPoint(speed_r, -30);
						cprintf("left:rotation\r\n");
					}
					else if (roam_state == ST_ROAM_OBSTACLE_RIGHT) {
						speed_setPoint(speed_l, 30);
						speed_setPoint(speed_r, 30);
						cprintf("right:rotation\r\n");
					}
					else if (roam_state == ST_ROAM_OBSTACLE_REAR) {
//						speed_setPoint(speed_l, 60);
//						speed_setPoint(speed_r, -60);
//						cprintf("rear:forward\r\n");
					}
					rotation_delay--;
				}
				else {
					roam_state = ST_ROAM_GO_AHEAD;
				}
			}
		}
	}
	else {
	}
}

void sh_roam_set_speed(speed_t * pspeed_l, speed_t * pspeed_r) {
	speed_l = pspeed_l;
	speed_r = pspeed_r;
}

void sh_roam_set_srf05(srf05_t * dev) {
	srf05 = dev;
}

void sh_roam_init(uint16_t period_ms) {
	roam_timer_id = timer_add_cb(roam_cb, NULL);
	timer_start_cb(roam_timer_id, period_ms, 0);
}

int sh_roam(int argc, char ** argv) {

	if (argc == 1) {
		roaming = !roaming;

		if (roaming)
			cprintf("Roaming activated\r\n");
		else {
			speed_setPoint(speed_l, 0);
			speed_setPoint(speed_r, 0);
			cprintf("Roaming deactivated\r\n");
		}
	}
	if (argc == 2) {
		if (argv[1][0] == '0') {
			roaming = 0;
			cprintf("Roaming deactivated\r\n");
			speed_setPoint(speed_l, 0);
			speed_setPoint(speed_r, 0);
		}
		if (argv[1][0] == '1') {
			roaming = 1;
			cprintf("Roaming activated\r\n");
		}
	}
	return 0;
}

