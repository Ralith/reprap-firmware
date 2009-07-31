#include "gcode.h"

#include <stdlib.h>

typedef struct gword 
{
	char letter;
	float value;
} gword;

/* Read a single gcode word (e.g. G92 or X7.4 */
gword* readword(char **point) 
{
	gword *ret = malloc(sizeof(gword));
	ret->letter = '\0';
	for(;; (*point)++) {
		switch(**point) {
		/* Ignore whitespace */
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			continue;

		/* Comment; end of input */
		case ';':
			/* TODO: Comments not an error */
			return NULL;

		default:
			break;
		}

		char c = **point;
		if(ret->letter == '\0') { /* We need a letter */
			if(c < 'A' || (c > 'Z' && c < 'a') || (c > 'z')) {
				/* Not a letter; invalid input */
				return NULL;
			}
			ret->letter = c;
			continue;
		} else {				/* We need a number */
			if(c < '0' || c > '9') {
				/* Not a number; invalid input */
				return NULL;
			}
			ret->value = strtod(*point, point);
			break;
		}
	}

	return ret;
}

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
void parse_gcode(char *block) 
{
	/* Used to convert input to a consistent state */
	static bool inches = FALSE;
	static bool relative = FALSE;
	static float x_last = 0, y_last = 0, z_last = 0;

	/* Used to track params */
	static ubyte m_last = 0;
	static ubyte g_last = 0;

	/* Safely initialize current instruction */
	instructions[inst_write].change_mask = 0;
	
	if(*block == '/') {			/* Block delete character */
		return;
	}

	gword *word;
	while((word = readword(&block))) {
		switch(word->letter) {
		case 'G':
			g_last = TO_APPROX_UBYTE(word->value);
			switch(g_last) {
			case INTERP_RAPID:
			case INTERP_LINEAR:
			case INTERP_ARC_CW:
			case INTERP_ARC_CCW:
				instructions[inst_write].interp = g_last;
				instructions[inst_write].change_mask |= CHANGE_INTERP;
				break;

			case 4:
				/* Dwell, needs param */
				continue;

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
				continue;

			default:
				goto unsupported;
			}
			break;

		case 'M':
			m_last = TO_APPROX_UBYTE(word->value);
			switch(m_last) {
			case EX_ON:
			case EX_REVERSE:
			case EX_OFF:
				instructions[inst_write].extrude_state = m_last;
				instructions[inst_write].change_mask |= CHANGE_EXTRUDE_STATE;
				break;
				
			case 104:
				/* Temp change, needs param */
				continue;
			case 105:
				/* TODO: Get temp */
				break;

			case 106:
			case 107:
				/* Fan control */
				goto unsupported;

			case 108:
				/* Extrude rate change, needs param */
				continue;

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
				instructions[inst_write].extrude_temp = word->value;
				instructions[inst_write].change_mask |= CHANGE_EXTRUDE_TEMP;
				break;

			case 108:
				instructions[inst_write].extrude_rate = word->value;
				instructions[inst_write].change_mask |= CHANGE_EXTRUDE_RATE;
				break;

			default:
				switch(g_last) {
				case 4:
					instructions[inst_write].dwell_secs = word->value;
					instructions[inst_write].change_mask |= CHANGE_DWELL_SECS;

				default:
					goto unsupported;
				}
			}
			break;
			

			/* TODO: Consider converting these to multiples of resolution */
		case 'X':
			instructions[inst_write].x = CONVERT(x, word->value);
			instructions[inst_write].change_mask |= CHANGE_X;
			break;

		case 'Y':
			instructions[inst_write].y = CONVERT(y, word->value);
			instructions[inst_write].change_mask |= CHANGE_Y;
			break;

		case 'Z':
			instructions[inst_write].z = CONVERT(z, word->value);
			instructions[inst_write].change_mask |= CHANGE_Z;
			break;

		case 'F':
			instructions[inst_write].feedrate = MAYBE_IN(word->value);
			instructions[inst_write].change_mask |= CHANGE_FEEDRATE;
			break;


		case 'N':
			/* Line number, ignore */
			continue;

			
		default:
		unsupported:
			/* TODO: Abort on unsupported gcode */
			break;
		}

		free(word);
	}
	
	if(*block != '\0') {		/* Parsing did not complete */
		/* TODO: Abort on error. */
	}

	/* Increment write index */
	ubyte nextwrite = (inst_write + 1) & INST_BUFFER_MASK;
	while(nextwrite == inst_read)
	{
		/* We caught up with execution, wait for it to move on */
	}
	inst_write = nextwrite;
}
