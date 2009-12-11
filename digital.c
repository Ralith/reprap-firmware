#include "digital.h"

#include <avr/io.h>

#include "platform.h"
#include "util.h"

bool dig_mode(pin_t pin, digmode_t mode) 
{
	const portid_t pid = pin_pid[pin];
	if(pid == PID_INVALID) {
		return FALSE;
	}
	BSET(*pid_ddr[pid], pin_offset[pin], (mode == OUTPUT) ? 1 : 0);
	return TRUE;
}

bool dig_write(pin_t pin, digstate_t state) 
{
	const portid_t pid = pin_pid[pin];
	if(pid == PID_INVALID) {
		return FALSE;
	}
	BSET(*pid_port[pid], pin_offset[pin], (state == HIGH) ? 1 : 0);
	return TRUE;
}

bool dig_toggle(pin_t pin) 
{
	const portid_t pid = pin_pid[pin];
	if(pid == PID_INVALID) {
		return FALSE;
	}
	*pid_port[pid] ^= BV(pin_offset[pin]);
	return TRUE;
}

digstate_t dig_read(pin_t pin) 
{
	const portid_t pid = pin_pid[pin];
	if(pid == PID_INVALID) {
		/* TODO: Indicate error */
		return LOW;
	}
	return (*pid_in[pid]) & BV(pin_offset[pin]);
}
