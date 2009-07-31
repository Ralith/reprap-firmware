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

#define TO_APPROX_BYTE(x) ((byte)(x + 0.1))
#define MAYBE_INCHES(x) (inches ? 25.4 * x : x)


/* Circular buffer of instructions to execute */
volatile inst_t instructions[INST_BUFFER_LEN];
volatile ubyte inst_read;
volatile ubyte inst_write;
void parse_gcode(char *block) 
{
	/* Used to convert input to a consistent state */
	static bool inches = FALSE;
	static bool relative = FALSE;
	
	if(*block == '/') {			/* Block delete character */
		return;
	}

	gword *word;
	while((word = readword(&block))) {
		switch(word->letter) {
		case 'G':
			switch(TO_APPROX_BYTE(word->value)) {
			case INTERP_RAPID:
			case INTERP_LINEAR:
			case INTERP_ARC_CW:
			case INTERP_ARC_CCW:
				instructions[inst_write].interp = TO_APPROX_BYTE(word->value);
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

			default:
				goto unsupported;
			}
			break;

		case 'M':
			/* TODO */
			break;

			
		case 'X':
			instructions[inst_write].x = MAYBE_INCHES(word->value);
			break;

		case 'Y':
			instructions[inst_write].y = MAYBE_INCHES(word->value);
			break;

		case 'Z':
			instructions[inst_write].z = MAYBE_INCHES(word->value);
			break;


		case 'N':
			/* Line number, ignore */
			continue;

			
		case 'T':
		unsupported:
			/* TODO: Abort on unsupported gcode */
			break;
		}

		free(word);
	}
	
	if(*block != '\0') {		/* Parsing did not complete */
		/* TODO: Abort on error. */
	}
}
