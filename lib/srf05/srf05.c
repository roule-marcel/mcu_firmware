#include "srf05.h"

#include <stdint.h>
#include <hardware/omsp_system.h>

#include <serial/cprintf/cprintf.h>

int srf05_init(srf05_t * dev, uint16_t per_addr) {
	dev->reg = (uint16_t*)per_addr;

	return 0;
}

uint16_t srf05_read_raw(srf05_t * dev, int num) {
	return dev->reg[num];
}

uint16_t srf05_read_mm(srf05_t * dev, int num) {
	uint32_t ret;
	ret = dev->reg[num];
	ret *= 10;
	ret /= 58;
	return (uint16_t) ret;
}
