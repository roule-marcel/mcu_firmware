#include "qei.h"

#include <stdint.h>
#include <hardware/omsp_system.h>

#include <serial/cprintf/cprintf.h>

int qei_init(qei_t * dev, uint16_t per_addr) {
	dev->reg = (uint32_t *)per_addr;

	return 0;
}

int qei_read_steps(qei_t * dev) {
	return * (dev->reg);
}