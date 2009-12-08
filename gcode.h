/* GCode parsing */

#ifndef _GCODE_H_
#define _GCODE_H_

#include "types.h"
#include "util.h"
#include "config.h"

/* Values correspond to G/M codes */
typedef enum interp
{
	INTERP_RAPID = 0,
	INTERP_LINEAR = 1,
	INTERP_ARC_CW = 2,
	INTERP_ARC_CCW = 3,
	INTERP_OFFSET				/* Not strictly an interpolation, but related */
} interp_t;

typedef enum extrude_state
{
	EX_ON = 101,
	EX_REVERSE = 102,
	EX_OFF = 103,
} extrude_state_t;

typedef enum inst_change
{
	CHANGE_INTERP = BV(0),
	CHANGE_POSITION = BV(1),
	CHANGE_FEEDRATE = BV(2),
	CHANGE_RADIUS = BV(3),
	CHANGE_EXTRUDE_RATE = BV(4),
	CHANGE_EXTRUDE_TEMP = BV(5),
	CHANGE_EXTRUDE_STATE = BV(6),
	CHANGE_DWELL_SECS = BV(7),
	CHANGE_GET_TEMP = BV(8),
} inst_change_t;

/* Instruction; represents one parsed gcode block */
typedef struct inst 
{
	uint16_t changes;
	
	uint8_t interp;
	int16_t position[AXES];
	/* TODO: Should these be ints as well? */
	float feedrate;
	float radius;				/* Center-style arcs are converted */
	float extrude_rate;
	float extrude_temp;
	uint8_t extrude_state;
	float dwell_secs;
} inst_t;

/* Return values of gcode parser funcs. <0 indicates an error*/
#define GCODE_BLOCK_COMPLETE 2
#define GCODE_WORD_COMPLETE 1
#define GCODE_SUCCESS 0
#define GCODE_INVALID_WORD -1

/* Circular buffer of instructions to execute */
#ifndef INST_BUFFER_LEN
#define INST_BUFFER_LEN 32
#endif
#define INST_BUFFER_MASK (INST_BUFFER_LEN - 1)
#if ( INST_BUFFER_LEN & INST_BUFFER_MASK )
#error Instruction buffer size is not a power of 2
#endif
extern volatile inst_t instructions[INST_BUFFER_LEN];
extern volatile uint8_t inst_read;
extern volatile uint8_t inst_write;

/* Parses and prepares for execution of the provided null-terminated block. */
int8_t gcode_parsew(const char letter, const float value);
int8_t gcode_parsec(const char c);

#endif
