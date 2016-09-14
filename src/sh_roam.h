#ifndef __SH_ROAM_H__
#define __SH_ROAM_H__

#include <srf05/srf05.h>

#include "speed.h"

void sh_roam_set_speed(speed_t * pspeed_l, speed_t * pspeed_r);
void sh_roam_set_srf05(srf05_t * dev);
void sh_roam_init(uint16_t period_ms);
int sh_roam(int argc, char ** argv);

#endif //__SH_ROAM_H__
