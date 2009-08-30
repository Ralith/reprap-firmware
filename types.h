/* Types to help readability */

#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdint.h>

typedef enum digmode {
	INPUT = 0,
	OUTPUT = 1
} digmode_t;

typedef enum digstate {
	LOW = 0,
	HIGH = 1
} digstate_t;

/* PORT[A-D] are used by avr-libc to refer to the corresponding
 * control registers */
typedef enum port {
	INVALID_PORT,
	_PORTA,
	_PORTB,
	_PORTC,
	_PORTD
} port_t;

/* See datasheet Figure 1-1 */
#define PIN_MIN 1
#define PIN_PORTB_MIN PIN_MIN
#define PIN_PORTB_MAX 8
#define PIN_PORTD_MIN 14
#define PIN_PORTD_MAX 21
#define PIN_PORTC_MIN 22
#define PIN_PORTC_MAX 29
#define PIN_PORTA_MIN 33
#define PIN_PORTA_MAX 40

/* Pins are identified by physical numbering (e.g. 1-40 on an
 * ATMega644p (Sanguino)) */
typedef uint8_t pin_t;

#define TRUE 1
#define FALSE 0
typedef uint8_t bool;

#endif
