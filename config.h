#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "types.h"

#define X_STEP_PIN 6
#define X_DIR_PIN 5
#define X_ENABLE_PIN 4
#define X_MIN_PIN 3
/* #define X_MAX_PIN 2 */
/* #error You must specify the X stepper pins. */

#define Y_STEP_PIN 6
#define Y_DIR_PIN 5
#define Y_ENABLE_PIN 4
#define Y_MIN_PIN 3
/* #define Y_MAX_PIN 2 */
/* #error You must specify the Y stepper pins. */

#define Z_STEP_PIN 6
#define Z_DIR_PIN 5
#define Z_ENABLE_PIN 4
#define Z_MIN_PIN 3
/* #define Z_MAX_PIN 2 */
/* #error You must specify the Z stepper pins. */

#define ENDSTOP_CLOSED LOW
#define ENDSTOP_OPEN HIGH

#define STEPPER_ENABLE_OFF LOW
#define STEPPER_ENABLE_ON HIGH

#define EXTRUDER_MOTOR_PWM_PIN 19
#define EXTRUDER_MOTOR_DIR_PIN 18
#define EXTRUDER_HEATER_PWM_PIN 20
#define EXTRUDER_HEATER_TEMP_PIN 40

#define DEBUG_LED_PIN 1

#endif
