#ifndef __SH_SPEED_H__
#define __SH_SPEED_H__

#include "speed.h"

#include <srf05/srf05.h>

void sh_speed_init();
void sh_speed_set_dev(speed_t * left, speed_t * right);
void sh_speed_set_srf05(srf05_t * dev);
int sh_speed(int argc, char ** argv);
int sh_speed_config(int argc, char ** argv);

void sh_speed_arrow_up();
void sh_speed_arrow_down();
void sh_speed_arrow_right();
void sh_speed_arrow_left();

#endif //__SH_SPEED_H__
