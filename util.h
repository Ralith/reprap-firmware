/* Useful generic things */
#ifndef _UTIL_H_
#define _UTIL_H_

#define BV(bit) (1 << (bit))

#define BSET(place, bit, state)											\
	if(state)															\
		place |= BV(bit);												\
	else																\
		place &= ~BV(bit)												\
			

#endif
