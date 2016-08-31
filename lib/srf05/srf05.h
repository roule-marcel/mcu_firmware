#ifndef __SRF05_H__
#define __SRF05_H__

#include <stdint.h>

typedef struct {
	uint16_t * reg;
} srf05_t;

int srf05_init(srf05_t * dev, uint16_t per_addr);
uint16_t srf05_read_raw(srf05_t * dev, int num);
uint16_t srf05_read_mm(srf05_t * dev, int num);

#endif //__SRF05_H__
