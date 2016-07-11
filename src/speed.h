#ifndef __SPEED_H__
#define __SPEED_H__

#include "pid.h"
//typedef struct {
//	int kp;
//	int ki;
//	int kd;
//	int setPoint;
//	int integrator;
//	int lastError;
//} pid_t;

void speed_init(pid_t * pid, int kp, int ki, int kd);

#endif //__SPEED_H__
