/* Convenience code for digital pin management */

#ifndef _IO_H_
#define _IO_H_

#include "types.h"

/* Functions return FALSE on error, TRUE otherwise */
bool dig_mode(pin_t pin, digmode_t mode);

bool dig_write(pin_t pin, digstate_t state);

/* Returns LOW on invalid pin */
digstate_t dig_read(pin_t pin);

#endif
