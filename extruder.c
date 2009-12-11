#include "extruder.h"

#include <stdlib.h>

#include "config.h"
#include "pwm.h"
#include "digital.h"
#include "analog.h"

uint16_t _target_temp = 0;
uint16_t _current_temp = 0;

void extruder_pid(const uint16_t sense) {
	/* TODO: Implement PID */
	pwm_set(EXTRUDER_HEATER_PWM_PIN, 0);
	/* TODO: Scale to celsius */
	_current_temp = sense;
}

void extruder_init() {
	pwm_init(EXTRUDER_MOTOR_PWM_PIN);
	pwm_init(EXTRUDER_HEATER_PWM_PIN);
	dig_mode(EXTRUDER_MOTOR_DIR_PIN, OUTPUT);
	adc_init(EXTRUDER_SENSOR_PIN, &extruder_pid);
}

void extruder_set_rate(const int16_t rate) {
	pwm_set(EXTRUDER_MOTOR_PWM_PIN, abs(rate));
	/* TODO: Verify that this isn't backwards. */
	dig_write(EXTRUDER_MOTOR_DIR_PIN, rate > 0);
}

void extruder_set_temp(const uint16_t celsius) {
	_target_temp = celsius;
}

uint16_t extruder_get_temp() {
	return _current_temp;
}
