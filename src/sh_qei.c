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

int sh_qei(int argc, char ** argv) {
	cprintf("%w %w %w %w\r\n", 
			(uint16_t)(*(qei_l->reg) >> 16),
			(uint16_t)(*(qei_l->reg)),
			(uint16_t)(*(qei_r->reg) >> 16),
			(uint16_t)(*(qei_r->reg)));

	return 0;
}
