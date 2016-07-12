#include "utils.h"

#include <stdlib.h>

#include <serial/cprintf/cprintf.h>

uint16_t read_uint16(char * buf, int* ok) {
	int i = 0;
	uint16_t ret = 0;
	int hex = 0;

	*ok = 0;

	while(buf[i] != 0)
	{
		if (buf[i] == 'x') {
			hex = 1;
		}
		else {
			if (hex)
				if (buf[i] >= '0' && buf[i] <= '9') {
					ret *= 16;
					ret += (buf[i] - '0');
				}
				else if (buf[i] >= 'a' && buf[i] <= 'f') {
					ret *= 16;
					ret += (buf[i] + 10 - 'a');
				}
				else if (buf[i] >= 'A' && buf[i] <= 'F') {
					ret *= 16;
					ret += (buf[i] + 10 - 'A');
				}
				else {
					cprintf("%c is not a number\r\n", buf[i]);
					ok = 0;
					return 0;
				}
				
			else {
				if (buf[i] >= '0' && buf[i] <= '9') {
					ret *= 10;
					ret += (buf[i] - '0');
				}
				else {
					cprintf("%c is not a number\r\n", buf[i]);
					ok = 0;
					return 0;
				}
			}
		}
		
		i++;
	}

	*ok = 1;
	return ret;
}

int16_t read_int16(char * buf, int* ok) {
	int i = 0;
	uint16_t ret = 0;
	int polarity = 1;

	*ok = 0;

	while(buf[i] != 0)
	{
		if (buf[i] == '-') {
			polarity = -1;
		}
		else {
			if (buf[i] >= '0' && buf[i] <= '9') {
				ret *= 10;
				ret += (buf[i] - '0');
			}
			else {
				cprintf("%c is not a number\r\n", buf[i]);
				ok = 0;
				return 0;
			}
		}
		
		i++;
	}

	*ok = 1;
	return polarity*ret;
}
