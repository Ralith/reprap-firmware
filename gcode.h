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

typedef enum inst_type
{
	INST_INTERP,				/* G0-G4 */
} inst_type_t;

/* Instruction */
typedef struct inst 
{
	inst_type_t type;
	union 
	{
		interp_t interp;
	};
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
