#include "digital.h"

#include <avr/io.h>

#include "platform.h"
#include "util.h"

bool dig_mode(pin_t pin, digmode_t mode) 
{
	if(pin_ddr[pin] == 0) {
		return FALSE;
	}
	BSET(*pin_ddr[pin], pin_offset[pin], (mode == OUTPUT) ? 1 : 0);
	return TRUE;
}

bool dig_write(pin_t pin, digstate_t state) 
{
	if(pin_port[pin] == 0) {
		return FALSE;
	}
	BSET(*pin_port[pin], pin_offset[pin], (state == HIGH) ? 1 : 0);
	return TRUE;
}

bool dig_toggle(pin_t pin) 
{
	if(pin_port[pin] == 0) {
		return FALSE;
	}
	*pin_port[pin] ^= BV(pin_offset[pin]);
	return TRUE;
}

digstate_t dig_read(pin_t pin) 
{
	if(pin_in[pin] == 0) {
		/* TODO: Indicate error */
		return LOW;
	}
	return (*pin_in[pin]) & BV(pin_offset[pin]);
}
