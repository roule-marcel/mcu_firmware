#ifndef __SH_QEI_H__
#define __SH_QEI_H__

#include <qei/qei.h>
#include <pwm/pwm.h>

void sh_qei_set_dev(qei_t * left, qei_t * right);
void sh_qei_set_pwm(pwm_t * left, pwm_t * right);
int sh_qei(int argc, char ** argv);

#endif //__SH_QEI_H__
