#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <stddef.h> 			/* size_t */

#include "types.h"

#define BAUD 19200

void ser_init();

void ser_write(ubyte b);

/* Convenience funcs */
/* buf must be null terminated */
void ser_write_str(char *buf);

#endif
