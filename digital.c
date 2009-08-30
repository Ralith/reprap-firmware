#include "digital.h"

#include <avr/io.h>

#include "util.h"

/* Sets a pin state at the correct corresponding register. */
inline bool pregset(volatile uint8_t *a, volatile uint8_t *b, volatile uint8_t *c, volatile uint8_t *d,
					pin_t pin, bool state) 
{
	/* Sanity insurance */
	if(state > 1) {
		state = 1;
	}
	
	portid_t port = pinport(pin);
	uint8_t offset = pinoffset(pin, port);
	switch(port) {
	case PID_A:
		/* PORTA is backwards for some reason, so we have to swap 7
		 * 7 with 0, 6 with 1, etc. */
		BSET(*a, offset, state);
		break;

	case PID_B:
		BSET(*b, offset, state);
		break;

	case PID_C:
		BSET(*c, pin - PIN_PORTC_MIN, state);
		break;

	case PID_D:
		BSET(*d, pin - PIN_PORTD_MIN, state);
		break;

	default:
		return FALSE;
	}
	return TRUE;
}

bool dig_mode(pin_t pin, digmode_t mode) 
{
	return pregset(&DDRA, &DDRB, &DDRC, &DDRD,
				   pin, (mode == OUTPUT) ? TRUE : FALSE);
}

bool dig_write(pin_t pin, digstate_t state) 
{
	return pregset(&PORTA, &PORTB, &PORTC, &PORTD,
				   pin, (state == HIGH ? TRUE : FALSE));
}

bool dig_toggle(pin_t pin) 
{
	portid_t port = pinport(pin);
	uint8_t offset = pinoffset(pin, port);
	switch(port) {
	case PID_A:
		PORTA ^= BV(offset);
		break;

	case PID_B:
		PORTB ^= BV(offset);
		break;

	case PID_C:
		PORTC ^= BV(offset);
		break;

	case PID_D:
		PORTD ^= BV(offset);
		break;

	default:
		return FALSE;
	}
	return TRUE;
}

digstate_t dig_read(pin_t pin) 
{
	portid_t port = pinport(pin);
	uint8_t offset = pinoffset(pin, port);
	switch(port) {
	case PID_A:
		return PINA & BV(offset);

	case PID_B:
		return PINB & BV(offset);

	case PID_C:
		return PINC & BV(offset);

	case PID_D:
		return PIND & BV(offset);

	default:
		break;
	}

	/* TODO: Indicate error */
	return LOW;
}
