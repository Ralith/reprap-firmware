/* Types to help readability */

#ifndef _TYPES_H_
#define _TYPES_H_

typedef enum digmode {
	INPUT = 0,
	OUTPUT = 1
} digmode_t;

typedef enum digstate {
	OFF = 0,
	ON = 1
} digstate_t;

typedef unsigned char pin_t;

#endif
