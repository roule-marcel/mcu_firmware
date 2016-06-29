#include "sh_reg.h"

#include <stdint.h>
 
#include <serial/cprintf/cprintf.h>

#include "utils.h"

// Write a value in a given register
int sh_reg_write(int argc, char ** argv) {
	uint16_t reg;
	uint16_t val;
	int ok;

	unsigned int * p;

	cprintf("write\n\r");

	if (argc < 3) {
		cprintf("error: missing argument\r\n");
		cprintf("correct usage:\r\n");
		cprintf("\t%s REGISTER VALUE\r\n", argv[0]);

		return -1;
	}

	reg = read_uint16(argv[1], &ok);
	val = read_uint16(argv[2], &ok);

	cprintf("r=%d v=%d\r\n", reg, val);

	p = (unsigned int *)reg;
	*p = val;

	return 0;
}

// Read and display the value of a given register
int sh_reg_read(int argc, char ** argv) {
	uint16_t reg;
	uint16_t val;
	int ok;

	unsigned int * p;

	if (argc < 2) {
		cprintf("error: missing argument\r\n");
		cprintf("correct usage:\r\n");
		cprintf("\t%s REGISTER\r\n", argv[0]);

		return -1;
	}

	cprintf("read\n\r");

	reg = read_uint16(argv[1], &ok);

	p = (unsigned int *)reg;
	val = *p;

	cprintf("r=%d v=%d\r\n", reg, val);

	return 0;
}
