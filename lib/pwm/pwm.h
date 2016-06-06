#ifndef __PWM_H__
#define __PWM_H__

#include <stdint.h>

typedef struct {
	uint16_t CTRL;
	uint16_t PERIOD;
	uint16_t DUTY;
	uint16_t DEAD_BAND;
} pwm_regs_t;

typedef struct {
	pwm_regs_t * regs;
	uint16_t freq_hz;
	uint16_t dead_band_us;
	float duty;
} pwm_t;

int pwm_init(pwm_t * dev, uint16_t per_addr, uint16_t freq_hz, uint16_t dead_band_us);
void pwm_enable(pwm_t * dev);
void pwm_set_duty(pwm_t * dev, float duty);

#endif //__PWM_H__
