#ifndef __SPEED_H__
#define __SPEED_H__

#include <pwm/pwm.h>
#include <qei/qei.h>

#include "pid.h"
//typedef struct {
//	int kp;
//	int ki;
//	int kd;
//	int setPoint;
//	int integrator;
//	int lastError;
//} pid_t;

typedef struct {
	pid_t pid;
	pwm_t * pwm;
	qei_t * qei;
	int32_t old_steps;
	uint16_t period_ms;
	int timer_id;
} speed_t;

void speed_init(speed_t * dev, pwm_t * pwm, qei_t * qei, uint16_t period_ms, float kp, float ki, float kd);
void speed_start(speed_t * dev);
void speed_stop(speed_t * dev);
void speed_setPoint(speed_t * dev, int setPoint);

//void pid_init(pid_t * pid, int kp, int ki, int kd);
//void pid_reset(pid_t * pid);
//void pid_kp(pid_t * pid, int kp);
//void pid_ki(pid_t * pid, int ki);
//void pid_kd(pid_t * pid, int kd);
//void pid_setPoint(pid_t * pid, int setPoint);
//int pid_compute(pid_t * pid, int measure);

#endif //__SPEED_H__
