#include "pid.h"

#include <serial/cprintf/cprintf.h>

void pid_init(pid_t * pid, float kp, float ki, float kd, float setPointIncrement) {
	pid_kp(pid, kp);
	pid_ki(pid, ki);
	pid_kd(pid, kd);

	pid_setPointIncrement(pid, setPointIncrement);
	pid->setPointFiltred = 0.0;

	pid_reset(pid);
}

void pid_reset(pid_t * pid) {
	pid->setPoint = 0;
	pid->integrator = 0;
	pid->lastError = 0;
}

void pid_kp(pid_t * pid, float kp) {
	pid->kp = kp;
}

void pid_ki(pid_t * pid, float ki) {
	pid->ki = ki;
}

void pid_kd(pid_t * pid, float kd) {
	pid->kd = kd;
}

void pid_setPoint(pid_t * pid, float setPoint) {
	pid->setPoint = setPoint;
}

void pid_setPointIncrement(pid_t * pid, float setPointIncrement) {
	pid->setPointIncrement = setPointIncrement;
}

char s[64];
float pid_compute(pid_t * pid, float measure) {
	float output;
	float error;
	float derivativeError;

	if (pid->setPointFiltred < pid->setPoint) {
		pid->setPointFiltred += pid->setPointIncrement;
		if (pid->setPointFiltred > pid->setPoint) {
			pid->setPointFiltred = pid->setPoint;
		}
	}
	else {
		pid->setPointFiltred -= pid->setPointIncrement;
		if (pid->setPointFiltred < pid->setPoint) {
			pid->setPointFiltred = pid->setPoint;
		}
	}

//	cprintf("%d %d\r\n", (int)(pid->setPoint), (int)(pid->setPointFiltred));

	error = pid->setPointFiltred - measure;
	pid->integrator += error;
	derivativeError = error - pid->lastError;

	output = error * pid->kp;
	output += pid->integrator * pid->ki;
	output += derivativeError * pid->kd;

	return output;
}
