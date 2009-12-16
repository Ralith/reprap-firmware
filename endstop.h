#include "types.h"

extern digstate_t endstops[2*AXES]; /* [xmin|xmax|ymin|ymax|zmin|zmax|...] */

void endstop_init();
