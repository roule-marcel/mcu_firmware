#include "sh_buzzer.h"

#include "utils.h"

buzzer_t * buzzer0;

void sh_buzzer_set_dev(buzzer_t * dev) {
	buzzer0 = dev;
}

// buzzer <DURATION> <PERIOD>
int sh_buzzer(int argc, char ** argv) {
	int ok;
	uint16_t duration = 100;
	uint16_t period = 4394;

	if (argc > 1) {
		duration = read_uint16(argv[1], &ok);
	}
	if (argc > 2) {
		period = read_uint16(argv[2], &ok);
	}

	buzzer(buzzer0, period, duration);

	return 0;
}
