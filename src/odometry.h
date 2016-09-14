#ifndef __ODOMETRY_H__
#define __ODOMETRY_H__

#include <stdint.h>

#include <qei/qei.h>

typedef struct {
	float wheel_radius;
	float demi_wheel_track;		// Radius from robot center to wheel
	int pulse_per_revolution;
} odometry_param_t;

typedef struct {
	float x;
	float y;
	float theta;
} odometry_position_t;

typedef struct {
	odometry_param_t param;
	odometry_position_t position;
	int32_t steps_left_old;
	int32_t steps_right_old;
	qei_t * qei_l;
	qei_t * qei_r;
	int timer_id;
} odometry_t;

void odometry_init(odometry_t * dev, float wheel_radius, float wheel_track, int pulse_per_revolution);
void odometry_set_qei(odometry_t * dev, qei_t * qei_r, qei_t * qei_l);
void odometry_compute(odometry_t * dev, int32_t steps_left, int32_t steps_right); 
void odometry_start(odometry_t * dev, uint16_t period_ms);
void odometry_stop(odometry_t * dev);

#endif //__ODOMETRY_H__
