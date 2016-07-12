#ifndef __PID_H__
#define __PID_H__

typedef struct {
	float kp;
	float ki;
	float kd;
	float setPoint;
	float integrator;
	float lastError;
} pid_t;

void pid_init(pid_t * pid, float kp, float ki, float kd);
void pid_reset(pid_t * pid);
void pid_kp(pid_t * pid, float kp);
void pid_ki(pid_t * pid, float ki);
void pid_kd(pid_t * pid, float kd);
void pid_setPoint(pid_t * pid, float setPoint);
float pid_compute(pid_t * pid, float measure);

#endif //__PID_H__
