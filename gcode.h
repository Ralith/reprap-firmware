/* GCode parsing */

#ifndef _GCODE_H_
#define _GCODE_H_

#include "types.h"

/* Values correspond to G codes */
typedef enum interp
{
	INTERP_RAPID = 0,
	INTERP_LINEAR = 1,
	INTERP_ARC_CW = 2,
	INTERP_ARC_CCW = 3,
} interp_t;

/* Instruction; represents one parsed gcode block */
typedef struct inst 
{
	interp_t interp;
	float x;
	float y;
	float z;
	float feedrate;
	float radius;				/* Center-style arcs are converted */
	float spindlerate;
	float extrder_temp;
	float dwell_secs;
} inst_t;

#ifndef INST_BUFFER_LEN
#define INST_BUFFER_LEN 16
#endif
/* Circular buffer of instructions to execute */
extern volatile inst_t instructions[INST_BUFFER_LEN];
extern volatile ubyte inst_read;
extern volatile ubyte inst_write;

/* Parses and prepares for execution of the provided null-terminated block. */
void parse_gcode(char *block);

#endif
