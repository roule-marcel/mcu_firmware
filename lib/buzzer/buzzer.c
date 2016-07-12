#include "buzzer.h"

#include <timer/timer.h>

int tim_id = -1;

void buzzer_stop(void *p) {
	*(uint16_t*)p = 0;
}


int buzzer_init(buzzer_t * dev, uint16_t per_addr) {
	dev->reg = (uint16_t*)per_addr;
	
	tim_id = timer_add_cb(buzzer_stop,(uint16_t*)per_addr);

	return 0;
}

void buzzer(buzzer_t * dev, uint16_t period, uint16_t duration) {
	*(dev->reg) = period;

	timer_start_cb(tim_id, duration, 1);
}
