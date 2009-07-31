#include "digital.h"

#include <avr/io.h>

#include "util.h"

bool dig_mode(pin_t pin, digmode_t mode) 
{
	/* See Figure 1-1 in datasheet */
	if(pin < PIN_MIN) {
		return FALSE;
	} else if(pin <= PIN_PORTB_MAX) { /* PORTB begins at PIN_MIN */
		BSET(DDRB, pin - PIN_PORTB_MIN, mode);
	} else if(PIN_PORTD_MIN <= pin && pin <= PIN_PORTD_MAX) {
		BSET(DDRD, pin - PIN_PORTD_MIN, mode);
	} else if(pin <= PIN_PORTC_MAX) { /* PORTC begins at PIN_PORTD_MAX + 1 */
		BSET(DDRC, pin - PIN_PORTC_MIN, mode);
	} else if(PIN_PORTA_MIN <= pin && pin <= PIN_PORTA_MAX) {
		/* PORTA is backwards for some reason, so we have to swap 7
		 * 7 with 0, 6 with 1, etc. */
		BSET(DDRA, (7 - (pin - PIN_PORTA_MIN)), mode);
	} else {
		return FALSE;
	}
	return TRUE;
}

bool dig_write(pin_t pin, digstate_t state) 
{
	/* See Figure 1-1 in datasheet */
	if(pin < PIN_MIN) {
		return FALSE;
	} else if(pin <= PIN_PORTB_MAX) { /* PORTB begins at PIN_MIN */
		BSET(PORTB, pin - PIN_PORTB_MIN, state);
	} else if(PIN_PORTD_MIN <= pin && pin <= PIN_PORTD_MAX) {
		BSET(PORTD, pin - PIN_PORTD_MIN, state);
	} else if(pin <= PIN_PORTC_MAX) { /* PORTC begins at PIN_PORTD_MAX + 1 */
		BSET(PORTC, pin - PIN_PORTC_MIN, state);
	} else if (PIN_PORTA_MIN <= pin && pin <= PIN_PORTA_MAX) {
		/* PORTA is backwards for some reason, so we have to swap 7
		 * 7 with 0, 6 with 1, etc. */
		BSET(PORTA, (7 - (pin - PIN_PORTA_MIN)), state);
	} else {
		return FALSE;
	}
	return TRUE;
}

digstate_t dig_read(pin_t pin) 
{
	/* See Figure 1-1 in datasheet */
	if(pin < PIN_MIN) {
		return LOW;
	} else if(pin <= PIN_PORTB_MAX) { /* PORTB begins at PIN_MIN */
		return (PINB & BV(pin - PIN_PORTB_MIN)) ? HIGH : LOW;
	} else if(PIN_PORTD_MIN <= pin && pin <= PIN_PORTD_MAX) {
		return (PIND & BV(pin - PIN_PORTD_MIN)) ? HIGH : LOW;
	} else if(pin <= PIN_PORTC_MAX) { /* PORTC begins at PIN_PORTD_MAX + 1 */
		return PINC & BV(pin - PIN_PORTC_MIN);
	} else if (PIN_PORTA_MIN <= pin && pin <= PIN_PORTA_MAX) {
		/* PORTA is backwards for some reason, so we have to swap 7
		 * 7 with 0, 6 with 1, etc. */
		return (PINA & BV(7 - (pin - PIN_PORTA_MIN))) ? HIGH : LOW;
	} else {
		return LOW;
	}
}
