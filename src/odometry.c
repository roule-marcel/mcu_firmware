#include "odometry.h"

#include <math.h>
#include <timer/timer.h>

// Add two angles and keep them in the ]-Pi;Pi] interval
float add_angle(float a, float b)
{
	if ((a + b) <= -M_PI)
		return 2*M_PI + a + b;
	else if ((a+b) > M_PI)
		return a + b - 2*M_PI;
	else
		return a + b;
}

void odometry_cb(void * p) {
	odometry_t * dev = (odometry_t*)p;

	odometry_compute(dev, -qei_read_steps(dev->qei_l), qei_read_steps(dev->qei_r));
}


void odometry_init(odometry_t * dev, float wheel_radius, float demi_wheel_track, int pulse_per_revolution) {
	dev->position.x = 0;
	dev->position.y = 0;
	dev->position.theta = 0;

	dev->param.wheel_radius = wheel_radius;
	dev->param.demi_wheel_track = demi_wheel_track;
	dev->param.pulse_per_revolution = pulse_per_revolution;

	dev->steps_left_old = 0;
	dev->steps_right_old = 0;

	dev->timer_id = timer_add_cb(odometry_cb,(void*)dev);
}

void odometry_set_qei(odometry_t * dev, qei_t * qei_r, qei_t * qei_l)
{
	dev->qei_r = qei_r;
	dev->qei_l = qei_l;
}

void odometry_compute(odometry_t * dev, int32_t steps_left, int32_t steps_right) {
	float dl, dr;
	float displacement;
	float delta = (2.0f*M_PI*dev->param.wheel_radius)/(dev->param.pulse_per_revolution);

	uint32_t dl_steps = steps_left - dev->steps_left_old;
	uint32_t dr_steps = steps_right - dev->steps_right_old;
	dl = dl_steps * delta;
	dr = dr_steps * delta;

	displacement = (dl+dr)/2.0f;

	/* No displacement */
	if (dl_steps == 0 && dr_steps == 0)
	{

	}
	/* Moving straight */
	else if (dl_steps == dr_steps)
	{
		dev->position.x += displacement * cos(dev->position.theta);
		dev->position.y += displacement * sin(dev->position.theta);
	}
	/* Pure rotation */
	else if (dl_steps == dr_steps)
	{
		float delta_theta = atan(dr/dev->param.demi_wheel_track);
		dev->position.theta = add_angle(dev->position.theta, delta_theta);
//		dev->position.theta += delta_theta;
	}
	/* Dans tous les autres cas */
	else
	{
		float radius;
		float delta_theta;
		float x_o, y_o;

		radius = dev->param.demi_wheel_track*(dl+dr)/(dr-dl);
		// delta_theta = deplacement/radius;
		delta_theta = atan((dr-dl)/(dev->param.demi_wheel_track*2.0f));
		x_o = dev->position.x - radius * sin(dev->position.theta);
		y_o = dev->position.y + radius * cos(dev->position.theta);

		dev->position.theta = add_angle(dev->position.theta, delta_theta);
//		dev->position.theta += delta_theta;
		dev->position.x = x_o + radius * sin(dev->position.theta);
		dev->position.y = y_o - radius * cos(dev->position.theta);
	}

	dev->steps_left_old = steps_left;
	dev->steps_right_old = steps_right;
}

void odometry_start(odometry_t * dev, uint16_t period_ms) {
	timer_start_cb(dev->timer_id, period_ms, 0);
}

void odometry_stop(odometry_t * dev) {
	timer_stop_cb(dev->timer_id);
}
