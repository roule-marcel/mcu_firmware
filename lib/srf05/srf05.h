#ifndef __SRF05_H__
#define __SRF05_H__

#include <stdint.h>

#define SRF05_FRONT_LEFT	0
#define SRF05_FRONT			1
#define SRF05_FRONT_RIGHT	2
#define SRF05_BACK_RIGHT	3
#define SRF05_BACK_LEFT		4

typedef struct {
	uint16_t * reg;
	uint16_t limits_mm[5];
} srf05_t;

int srf05_init(srf05_t * dev, uint16_t per_addr);
uint16_t srf05_read_raw(srf05_t * dev, int num);
uint16_t srf05_read_mm(srf05_t * dev, int num);
void srf05_set_limits_mm(srf05_t * dev, uint16_t * limits_mm);

#endif //__SRF05_H__
