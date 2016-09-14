#include "sh_odometry.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
 
#include <serial/cprintf/cprintf.h>

odometry_t * dev;

void sh_odometry_set_dev(odometry_t * pdev) {
	dev = pdev;
}

char s[64];
int sh_odometry(int argc, char ** argv) {
	sprintf(s, "x=%f y=%f t=%f", dev->position.x, dev->position.y, dev->position.theta);
	cprintf("%s\r\n", s);

	return 0;
}
