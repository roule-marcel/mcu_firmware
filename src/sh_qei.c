#include "sh_qei.h"

#include <stdint.h>
 
#include <serial/cprintf/cprintf.h>

#include "utils.h"

qei_t * qei_l;
qei_t * qei_r;

void sh_qei_set_dev(qei_t * left, qei_t * right) {
	qei_l = left;
	qei_r = right;
}

int sh_qei(char * buf) {
//	cprintf("%d %d\r\n", *qei_l->reg, *qei_r->reg);
	int32_t * p1;
	int32_t * p2;

	p1 = (int32_t*)0x0198;
	p2 = (int32_t*)0x019C;
	cprintf("%ld %ld\r\n", *p1, *p2);

	return 0;
}
