#ifndef __PID_H__
#define __PID_H__

typedef struct {
	int kp;
	int ki;
	int kd;
	int setPoint;
	int integrator;
	int lastError;
} pid_t;

void pid_init(pid_t * pid, int kp, int ki, int kd);
void pid_reset(pid_t * pid);
void pid_kp(pid_t * pid, int kp);
void pid_ki(pid_t * pid, int ki);
void pid_kd(pid_t * pid, int kd);
void pid_setPoint(pid_t * pid, int setPoint);
int pid_compute(pid_t * pid, int measure);

#endif //__PID_H__
