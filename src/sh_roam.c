#include "sh_roam.h"

#include <stdlib.h>
#include <timer/timer.h>

speed_t * speed_l;
speed_t * speed_r;
srf05_t * srf05;
int roaming = 0;
int timer_id;

void roam_cb (void * p) {
	if (roaming) {
		/**********************************************
		 * GO AHEAD MODE
		 **********************************************/

		// Every US is above threshold
		// Nothing dangerous
		// Go ahead
		
		// Back Left US or Back Right US under threshold
		// Someone probably following the robot
		// Stop a few seconds just in case then go ahead

		// Front US under threshold and Front Left US ~ Front Right US
		// There is something in front of the robot
		// Stop a few seconds then take a random angle

		// Front US under threshold and Front Left US << Front Right US
		// There is something in front left of the robot
		// Stop a few seconds then turn right

		// Front US under threshold and Front Left US >> Front Right US
		// There is something in front right of the robot
		// Stop a few seconds then turn left

		// Front Left US under threshold
		// Theres is something in front left of the robot
		// Stop a few seconds then turn right

		// Front Right US under threshold
		// Theres is something in front right of the robot
		// Stop a few seconds then turn left

		/**********************************************
		 * STOP MODE
		 **********************************************/

		// If obstacle disappeared then go ahead
		// Else take the appropriate angle

		/**********************************************
		 * ANGLE MODE
		 **********************************************/
	}
	else {
		speed_setPoint(speed_l, 0);
		speed_setPoint(speed_r, 0);
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
	timer_id = timer_add_cb(roam_cb, NULL);
	timer_start_cb(timer_id, period_ms, 0);
}

int sh_roam(int argc, char ** argv) {
	roaming = !roaming;
	return 0;
}

