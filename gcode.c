#include "gcode.h"

#include <stdlib.h>

#include "types.h"

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

volatile gc_state_t gc_state;
void parse_gcode(char *block) 
{
	if(*block == '/') {			/* Block delete character */
		return;
	}

	gword *word;
	while((word = readword(&block))) {
		switch(word->letter) {
		case 'G':
			switch(TO_APPROX_BYTE(word->value)) {
			case INTERP_RAPID:
				gc_state.interp_mode = INTERP_RAPID;
				break;
			case INTERP_LINEAR:
				gc_state.interp_mode = INTERP_LINEAR;
				break;
			case INTERP_ARC_CW:
				gc_state.interp_mode = INTERP_ARC_CW;
				break;
			case INTERP_ARC_CCW:
				gc_state.interp_mode = INTERP_ARC_CCW;
				break;

			default:
				goto unsupported;
			}
			break;

		case 'M':
			/* TODO */
			break;

			
		case 'X':
			/* TODO */
			break;

		case 'Y':
			/* TODO */
			break;

		case 'Z':
			/* TODO */
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
