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
	sprintf(s, "x=%d y=%d t=%d", (int)(dev->position.x*1000), (int)(dev->position.y*1000), (int)(dev->position.theta*1000));
	cprintf("%s\r\n", s);

	return 0;
}
