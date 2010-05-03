/* Convenience code for digital pin management */

#ifndef _IO_H_
#define _IO_H_

#include "types.h"
#include "platform.h"
#include "util.h"

/* Functions return FALSE on error, TRUE otherwise */
inline bool dig_mode(const pin_t pin, const digmode_t mode) 
{
	const portid_t pid = pin_pid[pin];
	if(pid == PID_INVALID) {
		return FALSE;
	}
	BSET(*pid_ddr[pid], pin_offset[pin], (mode == OUTPUT) ? 1 : 0);
	return TRUE;
}

inline bool dig_write(const pin_t pin, const digstate_t state) 
{
	const portid_t pid = pin_pid[pin];
	if(pid == PID_INVALID) {
		return FALSE;
	}
	BSET(*pid_port[pid], pin_offset[pin], (state == HIGH) ? 1 : 0);
	return TRUE;
}

inline bool dig_toggle(const pin_t pin) 
{
	const portid_t pid = pin_pid[pin];
	if(pid == PID_INVALID) {
		return FALSE;
	}
	*pid_port[pid] ^= BV(pin_offset[pin]);
	return TRUE;
}

/* Returns LOW on invalid pin */
inline digstate_t dig_read(const pin_t pin) 
{
	const portid_t pid = pin_pid[pin];
	if(pid == PID_INVALID) {
		/* TODO: Indicate error */
		return LOW;
	}
	return (*pid_in[pid]) & BV(pin_offset[pin]);
}

#endif
