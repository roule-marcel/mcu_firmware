#include "sh_reg.h"

#include <stdint.h>
 
#include <serial/cprintf/cprintf.h>

#include "utils.h"

// Write a value in a given register
int sh_reg_write(char * buf) {
	uint16_t reg;
	uint16_t val;
	int ok;

	unsigned int * p;

	cprintf("write\n\r");

	if (buf[1] != ' ' || buf[2] == 0) {
		cprintf("correct usage:\r\n");
		cprintf("\t%c REGISTER VALUE\r\n", buf[0]);
		return -1;
	}

	buf = &buf[2];
	buf = read_uint16(&reg, buf, &ok);
	if (ok == 0) {
		cprintf("Something went terribly wrong\r\n");
		return -1;
	}
	buf = read_uint16(&val, buf, &ok);
	if (ok == 0) {
		cprintf("Something went terribly wrong\r\n");
		return -1;
	}

	cprintf("r=%d v=%d\r\n", reg, val);

	p = (unsigned int *)reg;
	*p = val;

	return 0;
}

// Read and display the value of a given register
int sh_reg_read(char * buf) {
	uint16_t reg;
	uint16_t val;
	int ok;

	unsigned int * p;

	cprintf("read\n\r");

	if (buf[1] != ' ' || buf[2] == 0) {
		cprintf("correct usage:\r\n");
		cprintf("\t%c REGISTER\r\n", buf[0]);
		return -1;
	}

	buf = &buf[2];
	buf = read_uint16(&reg, buf, &ok);
	if (ok == 0) {
		cprintf("Something went terribly wrong\r\n");
		return -1;
	}

	p = (unsigned int *)reg;
	val = *p;

	cprintf("r=%d v=%d\r\n", reg, val);

	return 0;
}
