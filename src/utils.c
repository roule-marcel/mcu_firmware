#include "utils.h"

#include <stdlib.h>

#include <serial/cprintf/cprintf.h>

// read uint16 separated by spaces (' ') in a buffer.
// buf is the input buffer
// returns a pointer on the next value to extract.
// the uint16 is stored in the ret pointer.
// ok is 0 in case of an error, else 1
char * read_uint16(uint16_t * ret, char * buf, int * ok) {
	int i = 0;
	int hex = 0;

	*ret = 0;
	*ok = 0;

	while(buf[i] != 0)
	{
		if (buf[i] == 'x')
			hex = 1;
		else if (buf[i] == ' ') {
			*ok = 1;
			return &buf[i+1];
		}
		else {
			if (hex) {
				if (buf[i] >= '0' && buf[i] <= '9') {
					*ret *= 16;
					*ret += (buf[i] - '0');
				}
				else if (buf[i] >= 'a' && buf[i] <= 'f') {
					*ret *= 16;
					*ret += (buf[i] + 10 - 'a');
				}
				else if (buf[i] >= 'A' && buf[i] <= 'F') {
					*ret *= 16;
					*ret += (buf[i] + 10 - 'A');
				}
				else {
					cprintf("%c is not a number\r\n", buf[i]);
					return NULL;
				}
			}
			else {
				if (buf[i] >= '0' && buf[i] <= '9') {
					*ret *= 10;
					*ret += (buf[i] - '0');
				}
				else {
					cprintf("%c is not a number\r\n", buf[i]);
					return NULL;
				}
			}
		}
		i++;
	}

	*ok = 1;
	return NULL;
}
