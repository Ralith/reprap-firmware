/* GCode parsing */

#ifndef _GCODE_H_
#define _GCODE_H_

/* Values correspond to G codes */
typedef enum interp
{
	INTERP_RAPID = 0,
	INTERP_LINEAR = 1,
	INTERP_ARC_CW = 2,
	INTERP_ARC_CCW = 3,
} interp_t;

typedef struct gc_state
{
	interp_t interp_mode;
} gc_state_t;

extern volatile gc_state_t gc_state;

/* Parses and prepares for execution of the provided null-terminated block. */
void parse_gcode(char *block);

#endif
