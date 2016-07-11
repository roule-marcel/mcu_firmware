#ifndef __SH_SPEED_H__
#define __SH_SPEED_H__

#include "speed.h"

void sh_pid_set_dev(pid_t * left, pid_t * right);
int sh_speed(int argc, char ** argv);
int sh_speed_config(int argc, char ** argv);


#endif //__SH_SPEED_H__
