#ifndef __QEI_H__
#define __QEI_H__

#include <stdint.h>

typedef struct {
	uint16_t * reg;
} qei_t;

int qei_init(qei_t * dev, uint16_t per_addr);
int32_t qei_read_steps(qei_t * dev);

#endif //__QEI_H__
