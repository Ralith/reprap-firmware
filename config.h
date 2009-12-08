#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "types.h"

/* 0 indicates that there is no pin assigned to a given functionality. */
/* Note that STEP_PIN doubles as the "axis stepper used" boolean */
/* All axis-related arrays are of the form:
                               { X,    Y,    Z} */
#define STEPS_PER_MM ((float[]){10.0, 10.0, 10.0})
#define STEP_PIN   ((uint8_t[]){ 6,   10,   14})
#define DIR_PIN    ((uint8_t[]){ 5,    9,   13})
#define ENABLE_PIN ((uint8_t[]){ 4,    8,   12})
#define MIN_PIN    ((uint8_t[]){ 3,    7,   11})
#define MAX_PIN    ((uint8_t[]){ 0,    0,    0})

#define ENDSTOP_CLOSED LOW
#define ENDSTOP_OPEN HIGH
#define ENDSTOP_UNDEFINED -1

#define STEPPER_ENABLE_OFF LOW
#define STEPPER_ENABLE_ON HIGH

#define EXTRUDER_MOTOR_PWM_PIN 19
#define EXTRUDER_MOTOR_DIR_PIN 18
#define EXTRUDER_HEATER_PWM_PIN 20
#define EXTRUDER_SENSOR_PIN 40
#define EXTRUDER_SENSOR_THERMOCOUPLE
/* #error You forgot to configure the firmware! Comment this out when you're done. */

#define DEBUG_LED_PIN 1

#define DEFAULT_FEEDRATE 60		 /* cm/minute; TODO: Re-evaluate unit choice */
#define DEFAULT_EXTRUDE_RATE 100 /* PWM duty cycle; TODO: Stepstruder support */

#endif
