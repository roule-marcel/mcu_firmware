#include "pid.h"

//typedef struct {
//	int kp;
//	int ki;
//	int kd;
//	int setPoint;
//	int integrator;
//	int lastError;
//} pid_t;

void pid_init(pid_t * pid, int kp, int ki, int kd) {
	pid_kp(pid, kp);
	pid_ki(pid, ki);
	pid_kd(pid, kd);

	pid_reset(pid);
}

void pid_reset(pid_t * pid) {
	pid->setPoint = 0;
	pid->integrator = 0;
	pid->lastError = 0;
}

void pid_kp(pid_t * pid, int kp) {
	pid->kp = kp;
}

void pid_ki(pid_t * pid, int ki) {
	pid->ki = ki;
}

void pid_kd(pid_t * pid, int kd) {
	pid->kd = kd;
}

void pid_setPoint(pid_t * pid, int setPoint) {
	pid->setPoint = setPoint;
}

int pid_compute(pid_t * pid, int measure) {
	int output;
	int error;
	int derivativeError;

	error = pid->setPoint - measure;
	pid->integrator += error;
	derivativeError = error - pid->lastError;

	output = error * pid->kp;
	output += pid->integrator * pid->ki;
	output += derivativeError * pid->kd;

	return output;
}
