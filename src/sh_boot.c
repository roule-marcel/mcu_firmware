#include "sh_boot.h"

#include <stdint.h>

#include <serial/cprintf/cprintf.h>

int sh_bootloader(int argc, char ** argv) {
	uint16_t * p_boot;
	p_boot = (uint16_t*)0x0190;

	p_boot[1] = 0x8000;		// ADDRESS
// BAUD FREQ = 16*baud_rate / gcd(global_clock_freq, 16*baud_rate) = 16*9600/38400 = 4
// BAUD LIMIT = (global_clock_freq / gcd(global_clock_freq, 16*baud_rate)) - baud_freq = 24000000/38400 - 4 = 621 = 0x26D
// 9600
//	p_boot[2] = 0x0004;		// BAUD_FREQ
//	p_boot[3] = 0x026D;		// BAUD_LIMIT
// 115200
	p_boot[2] = 0x0030;		// BAUD_FREQ
	p_boot[3] = 0x0241;		// BAUD_LIMIT

	cprintf("Waiting for a new .bin file\r\n");

	p_boot[0] = 0x0001;		// EN
	cprintf("We should not see that\r\n");

	return 0;
}
