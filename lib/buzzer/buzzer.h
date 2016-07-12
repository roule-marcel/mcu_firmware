#ifndef __BUZZER_H__
#define __BUZZER_H__

#include <stdint.h>

typedef struct {
	uint16_t * reg;
} buzzer_t;

int buzzer_init(buzzer_t * dev, uint16_t per_addr);
void buzzer(buzzer_t * dev, uint16_t period, uint16_t duration);

#endif //__BUZZER_H__
