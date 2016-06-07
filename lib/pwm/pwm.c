#include "pwm.h"

#include <stdint.h>
#include <hardware/omsp_system.h>

#include <serial/cprintf/cprintf.h>

#include "pwm_regs.h"

int pwm_init(pwm_t * dev, uint16_t per_addr, uint16_t freq_hz, uint16_t dead_band_us) {
	dev->regs = (pwm_regs_t*)per_addr;

	dev->freq_hz = freq_hz;
	dev->dead_band_us = dead_band_us;

	dev->regs->PERIOD = CPU_FREQ_HZ/dev->freq_hz;
	dev->regs->DEAD_BAND = dev->dead_band_us*(CPU_FREQ_HZ/1000000);

	return 0;
}

void pwm_enable(pwm_t * dev) {
	pwm_set_duty(dev, 0.5);
	dev->regs->CTRL = PWM_EN;
}

void pwm_set_duty(pwm_t * dev, float duty) {
	uint16_t reg_duty;
	dev->duty = duty;

	reg_duty = (dev->regs->PERIOD*dev->duty) - dev->regs->DEAD_BAND;
	dev->regs->DUTY = reg_duty;
}
