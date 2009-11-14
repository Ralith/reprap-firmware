/* Useful generic things */
#ifndef _UTIL_H_
#define _UTIL_H_

#include "types.h"

/* Bitmask for the given index */
#define BV(bit) (1 << (bit))

/* Sets index 'bit' at 'place' to 'state'  */
#define BSET(place, bit, state)											\
	if(state)															\
		place |= BV(bit);												\
	else																\
		place &= ~BV(bit)

inline portid_t pinport(pin_t pin) {
	if(pin < PIN_MIN) {
		return PID_INVALID;
	} else if(pin <= PIN_PORTB_MAX) { /* PORTB begins at PIN_MIN */
		return PID_B;
	} else if(PIN_PORTD_MIN <= pin && pin <= PIN_PORTD_MAX) {
		return PID_D;
	} else if(pin <= PIN_PORTC_MAX) { /* PORTC begins at PIN_PORTD_MAX + 1 */
		return PID_C;
	} else if (PIN_PORTA_MIN <= pin && pin <= PIN_PORTA_MAX) {
		return PID_A;
	}
	return PID_INVALID;
}

/* Pass 0 to port to autodetect. */
inline uint8_t pinoffset(pin_t pin, portid_t port) {
	if(port == 0) {
		port = pinport(pin);
	}

	switch(port) {
	case PID_A:
		/* PORTA is backwards for some reason, so we have to swap 7
		 * 7 with 0, 6 with 1, etc. */
		return 7 - (pin - PIN_PORTA_MIN);

	case PID_B:
		return pin - PIN_PORTB_MIN;

	case PID_C:
		return pin - PIN_PORTC_MIN;

	case PID_D:
		return pin - PIN_PORTD_MIN;

	default:
		/* TODO: Freak out because pin is invalid or MCU is unsupported. */
		break;
	}

	/* TODO: Indicate error */
	return 0;
}

#endif
