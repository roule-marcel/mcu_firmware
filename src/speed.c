#include "speed.h"

#include <timer/timer.h>

#include <serial/cprintf/cprintf.h>

char s[64];
void speed_compute_cb(void * p) {
	int32_t steps;
	float command;
	float measure=0;
	float duty;
	speed_t * dev;
	dev = (speed_t*)p;

	steps = qei_read_steps(dev->qei);
	measure = steps - dev->old_steps;
	dev->old_steps = steps;

	command = pid_compute(&(dev->pid), measure);

	duty = command;
	duty += 0.5;
	if (duty > 0.9) duty = 0.9;
	if (duty < 0.1) duty = 0.1;

	pwm_set_duty(dev->pwm, duty);

//	cprintf("%d %d %d %d\r\n", (int)(dev->pid.setPoint), (int)(measure), (int)command, (int)(duty*100));
}

void speed_init(speed_t * dev, pwm_t * pwm, qei_t * qei, uint16_t period_ms, float kp, float ki, float kd, float increment) {
	dev->pwm = pwm;
	dev->qei = qei;
	// Acceleration ramp is configured through the increment value
	pid_init(&(dev->pid), kp, ki, kd, increment);
	dev->period_ms = period_ms;

	dev->timer_id = timer_add_cb(speed_compute_cb,(void*)dev);
}

void speed_start(speed_t * dev) {
	dev->old_steps = qei_read_steps(dev->qei);
	timer_start_cb(dev->timer_id, dev->period_ms, 0);
}

void speed_stop(speed_t * dev) {
	pwm_set_duty(dev->pwm, 0.5);
	timer_stop_cb(dev->timer_id);
}
 
void speed_setPoint(speed_t * dev, int setPoint) {
	pid_setPoint(&(dev->pid), (float)setPoint);
}
