#include "timer.h"

#include <hardware/omsp_system.h>

typedef struct{
	int running;
	uint16_t counter;
	uint16_t period_ms;
	void (* func)(void *);
	void * param;
} timer_func_t;

int timer_func_list_size = 0;
timer_func_t timer_func_list[_TIMER_FUNC_LIST_MAX_SIZE];

int timer_init() {
	// Clock source SMCLK (cause why not?) ; Input devider /8 ; Up mode (count from 0 to TACCR0) ; Enable Interrupts
	TACTL |= TASSEL1 | ID1 | ID0 | MC0 | TAIE;
	TACCR0 = 3000;

	return 0;
}

int timer_add_cb(uint16_t period_ms, void (* pfunc)(void *), void * param) {
	int id = timer_func_list_size;

	if (timer_func_list_size < _TIMER_FUNC_LIST_MAX_SIZE) {
		timer_func_list[timer_func_list_size].running = 0;
		timer_func_list[timer_func_list_size].counter = 0;
		timer_func_list[timer_func_list_size].period_ms = period_ms;
		timer_func_list[timer_func_list_size].func = pfunc;
		timer_func_list[timer_func_list_size].param = param;
		timer_func_list_size++;
		return id;
	}
	
	return -1;
}

int timer_start_cb(int id) {
	timer_func_list[id].running = 1;

	return 0;
}

int timer_stop_cb(int id) {
	timer_func_list[id].running = 0;

	return 0;
}

interrupt (TIMERA1_VECTOR) /*enablenested*/ INT_Timer_overflow(void) {
	int i;
	for (i = 0 ; i < timer_func_list_size ; i++) {
		if (timer_func_list[i].running) {
			if (timer_func_list[i].counter < timer_func_list[i].period_ms) {
				timer_func_list[i].counter++;
			}
			else {
				timer_func_list[i].counter = 0;
				timer_func_list[i].func(timer_func_list[i].param);
			}
		}
	}

//	static uint8_t blink = 0;
//	static uint16_t cnt = 0;
//
//	if (cnt > 1000)
//	{
//		// I'm pretty sure this is self-explanatory
//		P3OUT = 0xFF * blink;
//		blink = !blink;
//		cnt = 0;
//	}
//	cnt++;

	// Clear Interrupt
	TACTL &= ~TAIFG;
}
