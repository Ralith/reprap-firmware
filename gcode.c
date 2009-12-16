#include "gcode.h"

#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <avr/pgmspace.h>

#include "uart.h"

#include "config.h"

#ifndef NAN
#error This code requires an implementation with floating point NAN support
#endif

/* TODO: Minimize use of FP math */
#define TO_APPROX_UBYTE(x) ((uint8_t)(x + 0.1))
#define MAYBE_IN(position) (inches ? 25.4 * position : position)

/* Circular buffer of instructions to execute */
volatile inst_t instructions[INST_BUFFER_LEN];
volatile uint8_t inst_read;
volatile uint8_t inst_write;

static int32_t last_position[AXES];
static float last_extrude_rate = DEFAULT_EXTRUDE_RATE;

void gcode_init(void) 
{
	inst_write = 0;
	inst_read = 0;
	instructions[inst_write].changes = 0;
	uint8_t i;
	for(i = 0; i < AXES; i++) {
		last_position[i] = 0;
	}
}

int8_t gcode_parsew(const char letter, const float value) 
{
	/* Cross-call state tracking */
	/* Used to convert input to a consistent state */
	static bool inches = FALSE;
	static bool relative = FALSE;

	/* Used to track params */
	static uint8_t m_last = 0;
	static uint8_t g_last = 0;

	/* Aids converting I/J (center) arcs to R (radius) arcs */
	static float arc_i, arc_j;
	arc_i = NAN;
	arc_j = NAN;
	
	switch(letter) {
	case 'G':
		g_last = TO_APPROX_UBYTE(value);
		switch(g_last) {
		case INTERP_RAPID:
		case INTERP_LINEAR:
		case INTERP_ARC_CW:
		case INTERP_ARC_CCW:
			instructions[inst_write].interp = g_last;
			instructions[inst_write].changes |= CHANGE_INTERP;
			break;

		case 4:
			break;

		case 20:
			inches = TRUE;
			break;
		case 21:
			inches = FALSE;
			break;

		case 90:
			relative = FALSE;
			break;
		case 91:
			relative = TRUE;
			break;

		case 92:
			/* TODO: Set current offset */
			break;

		default:
			goto unsupported;
		}
		break;

	case 'M':
		m_last = TO_APPROX_UBYTE(value);
		switch(m_last) {
		case 101:
			instructions[inst_write].extrude_rate = last_extrude_rate;
			instructions[inst_write].changes |= CHANGE_EXTRUDE_RATE;
			break;
			
		case 102:
			instructions[inst_write].extrude_rate = -last_extrude_rate;
			instructions[inst_write].changes |= CHANGE_EXTRUDE_RATE;
			break;

		case 103:
			instructions[inst_write].extrude_rate = 0;
			instructions[inst_write].changes |= CHANGE_EXTRUDE_RATE;
			break;
				
		case 104:
			/* Temp change, needs param */
			break;
		case 105:
			instructions[inst_write].changes |= CHANGE_GET_TEMP;
			/* TODO: Get temp */
			break;

		case 106:
		case 107:
			/* Fan control */
			goto unsupported;

		case 108:
			/* Extrude rate change, needs param */
			break;

		case 120:
		case 121:
		case 122:
		case 123:
		case 124:
			/* Heater PID tweak */
			goto unsupported;

		default:
			goto unsupported;
		}
		break;

		/* These don't really seem to need differentiating */
	case 'S':
	case 'P':
		switch(m_last) {
		case 104:
			instructions[inst_write].extrude_temp = value;
			instructions[inst_write].changes |= CHANGE_EXTRUDE_TEMP;
			break;

		case 108:
			instructions[inst_write].extrude_rate = value;
			instructions[inst_write].changes |= CHANGE_EXTRUDE_RATE;
			last_extrude_rate = value;
			break;

		default:
			switch(g_last) {
			case 4:
				instructions[inst_write].dwell_ms = value * 1000.0;
				instructions[inst_write].changes |= CHANGE_DWELL_SECS;

			default:
				goto unsupported;
			}
		}
	break;			

	case 'X':
	case 'Y':
	case 'Z':
		{
			const uint8_t axis = letter - 'X';
			/* Convert from real to machine space */
			const int32_t position = (int)(MAYBE_IN(value) * STEPS_PER_MM[axis]) + (relative ? last_position[axis] : 0);
			instructions[inst_write].position[axis] = position;
			instructions[inst_write].changes |= CHANGE_POSITION;
			last_position[axis] = position;
		}
		break;

	case 'F':
		instructions[inst_write].feedrate = MAYBE_IN(value);
		instructions[inst_write].changes |= CHANGE_FEEDRATE;
		break;

	case 'R':
		instructions[inst_write].radius = MAYBE_IN(value);
		instructions[inst_write].changes |= CHANGE_RADIUS;
		break;
	case 'I':
		instructions[inst_write].changes |= CHANGE_RADIUS;
		if(isnan(arc_j)) {
			instructions[inst_write].radius = MAYBE_IN(value);
			arc_i = instructions[inst_write].radius;
		} else {
			instructions[inst_write].radius = hypot(MAYBE_IN(value), arc_j);
		}
		break;
	case 'J':
		instructions[inst_write].changes |= CHANGE_RADIUS;
		if(isnan(arc_i)) {
			instructions[inst_write].radius = MAYBE_IN(value);
			arc_j = instructions[inst_write].radius;
		} else {
			instructions[inst_write].radius = hypot(arc_i, MAYBE_IN(value));
		}
		break;


	case 'N':
		/* Line number, ignore */
		break;

			
	default:
	unsupported:
		return GCODE_INVALID_WORD;
		break;
	}
	return GCODE_SUCCESS;
}

/* Parse a single character */
int8_t gcode_parsec(const char c) 
{
	/* Cross-call state tracking */
	static char word_letter = '\0';
	/* 32 chars of number should be enough for anyone. */
	static char word_value[32];
	static uint8_t word_value_pos = 0;
	static bool ignore_block = FALSE;
	/* Number of chars into the block */
	static uint16_t index = 0;

	if(ignore_block) {
		if(c == '\r' || c == '\n') {
			/* Block over, stop ignoring */
			ignore_block = FALSE;
		}
		return GCODE_SUCCESS;
	}

	switch(c) {
	case '/':
		if(index == 0) {
			ignore_block = TRUE;
			break;
		} else {
			ignore_block = TRUE;
			return GCODE_INVALID_WORD;
		}
		break;
		
	case ' ':
	case '\t':
		/* Ignore whitespace */
		break;

	case ';':
		/* Skip remainder of block and parse what we have so far.
		 * Note absence of break. */
		ignore_block = TRUE;
	case '\r':
	case '\n':
		if(index != 0) { /* Block complete */
			/* Increment write index */
			uint8_t nextwrite = (inst_write + 1) & INST_BUFFER_MASK;
			while(nextwrite == inst_read)
			{
				/* We caught up with instruction execution, wait for it to move on */
			}
			inst_write = nextwrite;

			/* Initialize */
			instructions[inst_write].changes = 0;
			word_letter = '\0';
			word_value_pos = 0;
			index = 0;
			return GCODE_BLOCK_COMPLETE;
		} else {
			/* Don't let index be incremented for empty lines */
			return GCODE_SUCCESS;
		}
		break;

	default:
	{
		static bool got_point = FALSE;
		if(word_letter == '\0') {
			word_letter = c;
		} else if(isdigit(c)) {
			word_value[word_value_pos++] = c;
		} else if(!got_point && c == '.') {
			word_value[word_value_pos++] = c;
			got_point = TRUE;
		} else {
			/* Got a full word, interpret it */
			got_point = FALSE;
			static char *endptr;
			static int8_t result;
			result = gcode_parsew(word_letter, strtod(word_value, &endptr));
			/* Reset parsing state for next word. */
			word_value_pos = 0;
			word_letter = c;
			/* If we failed to parse a number or the word parsing failed... */
			if(endptr == word_value || result < 0) {
				/* ...let the caller know. */
				return GCODE_INVALID_WORD;
			}
			return GCODE_WORD_COMPLETE;
		}
		
		break;
	}
	}
	
	index++;
	return GCODE_SUCCESS;
}
