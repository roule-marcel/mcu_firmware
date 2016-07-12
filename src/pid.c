#include "pid.h"

void pid_init(pid_t * pid, float kp, float ki, float kd) {
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

float pid_compute(pid_t * pid, float measure) {
	float output;
	float error;
	float derivativeError;

	error = pid->setPoint - measure;
	pid->integrator += error;
	derivativeError = error - pid->lastError;

	output = error * pid->kp;
	output += pid->integrator * pid->ki;
	output += derivativeError * pid->kd;

	return output;
}
