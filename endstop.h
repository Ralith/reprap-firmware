#include "types.h"

#define ENDSTOP_COUNT (2*AXES)

extern volatile digstate_t endstops[ENDSTOP_COUNT]; /* [xmin|xmax|ymin|ymax|zmin|zmax|...] */

void endstop_init(void);
