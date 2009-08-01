#include "gcode.h"

#include <stdlib.h>

#define TO_APPROX_UBYTE(x) ((ubyte)(x + 0.1))
#define MAYBE_IN(x) (inches ? 25.4 * x : x)
#define MAYBE_REL(name, x) (relative ? x + name ## _last : x)
#define CONVERT(name, x) MAYBE_REL(name, MAYBE_IN(x))

#define INST_BUFFER_MASK (INST_BUFFER_LEN - 1)
#if ( INST_BUFFER_LEN & INST_BUFFER_MASK )
#error Instruction buffer size is not a power of 2
#endif

/* Circular buffer of instructions to execute */
volatile inst_t instructions[INST_BUFFER_LEN];
volatile ubyte inst_read;
volatile ubyte inst_write;

void gcode_parsew(const char letter, const float value) 
{
	/* Used to convert input to a consistent state */
	static bool inches = FALSE;
	static bool relative = FALSE;
	static float x_last = 0, y_last = 0, z_last = 0;

	/* Used to track params */
	static ubyte m_last = 0;
	static ubyte g_last = 0;

	switch(letter) {
	case 'G':
		g_last = TO_APPROX_UBYTE(value);
		switch(g_last) {
		case INTERP_RAPID:
		case INTERP_LINEAR:
		case INTERP_ARC_CW:
		case INTERP_ARC_CCW:
			instructions[inst_write].interp = g_last;
			instructions[inst_write].change_mask |= CHANGE_INTERP;
			break;

		case 4:
			/* Dwell, TODO */
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
			instructions[inst_write].interp = INTERP_OFFSET;
			instructions[inst_write].change_mask |= CHANGE_INTERP;
			break;

		default:
			goto unsupported;
		}
		break;

	case 'M':
		m_last = TO_APPROX_UBYTE(value);
		switch(m_last) {
		case EX_ON:
		case EX_REVERSE:
		case EX_OFF:
			instructions[inst_write].extrude_state = m_last;
			instructions[inst_write].change_mask |= CHANGE_EXTRUDE_STATE;
			break;
				
		case 104:
			/* Temp change, needs param */
			break;
		case 105:
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
			instructions[inst_write].change_mask |= CHANGE_EXTRUDE_TEMP;
			break;

		case 108:
			instructions[inst_write].extrude_rate = value;
			instructions[inst_write].change_mask |= CHANGE_EXTRUDE_RATE;
			break;

		default:
			switch(g_last) {
			case 4:
				instructions[inst_write].dwell_secs = value;
				instructions[inst_write].change_mask |= CHANGE_DWELL_SECS;

			default:
				goto unsupported;
			}
		}
	break;
			

	/* TODO: Consider converting these to multiples of resolution */
	case 'X':
		instructions[inst_write].x = CONVERT(x, value);
		instructions[inst_write].change_mask |= CHANGE_X;
		break;

	case 'Y':
		instructions[inst_write].y = CONVERT(y, value);
		instructions[inst_write].change_mask |= CHANGE_Y;
		break;

	case 'Z':
		instructions[inst_write].z = CONVERT(z, value);
		instructions[inst_write].change_mask |= CHANGE_Z;
		break;

	case 'F':
		instructions[inst_write].feedrate = MAYBE_IN(value);
		instructions[inst_write].change_mask |= CHANGE_FEEDRATE;
		break;

	case 'R':
		instructions[inst_write].radius = MAYBE_IN(value);
		instructions[inst_write].change_mask |= CHANGE_RADIUS;

	case 'I':
	case 'J':
		/* TODO: Convert to R */
		break;


	case 'N':
		/* Line number, ignore */
		break;

			
	default:
	unsupported:
		/* TODO: Abort on unsupported gcode */
		break;
	}
}

/* Parse a single character */
void gcode_parsec(char c) 
{
	static char word_letter = '\0';
	static char word_value[128];
	static ubyte word_value_pos = 0;
	static bool ignore_block = FALSE;
	/* Number of chars into the block */
	static unsigned short index = 0;

	if(ignore_block) {
		if(c == '\r' || c == '\n') {
			/* Block over, stop ignoring */
			ignore_block = FALSE;
		}
		return;
	}

	switch(c) {
	case '/':
		if(index == 0) {
			ignore_block = TRUE;
			break;
		} else {
			/* TODO: Error */
		}
		break;
		
	case ' ':
	case '\t':
		/* Ignore whitespace */
		break;

	case '\r':
	case '\n':
	case ';':
		if(index != 0) { /* Block complete */
			/* Increment write index */
			ubyte nextwrite = (inst_write + 1) & INST_BUFFER_MASK;
			while(nextwrite == inst_read)
			{
				/* We caught up with instruction execution, wait for it to move on */
			}
			inst_write = nextwrite;

			/* Initialize */
			instructions[inst_write].change_mask = 0;
			word_letter = '\0';
			word_value_pos = 0;
			index = 0;
			return;
		}
		break;

	default:
		if(word_letter == '\0') {
			word_letter = c;
		} else {
			if(c >= '0' && c <= '9') {
				word_value[word_value_pos++] = c;
			} else {
				char *endptr;
				gcode_parsew(word_letter, strtod(word_value, endptr));
				if(endptr == word_value) {
					/* TODO: Error */
				}
				word_value_pos = 0;
				word_letter = '\0';
			}
		}
		break;
	}
	
	index++;
}
