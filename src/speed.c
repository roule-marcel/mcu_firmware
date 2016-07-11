#include "speed.h"

void speed_init(pid_t * pid, int kp, int ki, int kd) {
	pid_init(pid, kp, ki, kd);
}
