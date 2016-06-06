#ifndef __SH_PWM_H__
#define __SH_PWM_H__

#include <pwm/pwm.h>

void set_pwm_dev(pwm_t * left, pwm_t * right);
int sh_pwm(char * buf);

#endif //__SH_PWM_H__
