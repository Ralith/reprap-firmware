/* Types to help readability */

#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdint.h>

typedef enum {
	INPUT = 0,
	OUTPUT = 1
} digmode_t;

typedef enum {
	LOW = 0,
	HIGH = 1
} digstate_t;

typedef enum {
	PID_INVALID = 0,
	PID_A,
	PID_B,
	PID_C,
	PID_D
} portid_t;

typedef enum {
	AXIS_X = 0,
	AXIS_Y = 1,
	AXIS_Z = 2,
	AXES
} axis_t;

/* Pins are identified by physical numbering (e.g. 1-40 on an
 * ATMega644p (Sanguino)) */
typedef uint8_t pin_t;

#define TRUE 1
#define FALSE 0
typedef uint8_t bool;

#endif
