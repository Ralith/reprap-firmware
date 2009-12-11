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

#endif
