#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>

#define _TIMER_FUNC_LIST_MAX_SIZE 32

int timer_init();
int timer_add_cb(void (* pfunc)(void *), void * param);
int timer_start_cb(int id, uint16_t period_ms);
int timer_stop_cb(int id);

#endif //__TIMER_H__
