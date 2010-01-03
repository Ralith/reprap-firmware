#include "types.h"
#include "line.h"

typedef struct linestate {
	int32_t to[AXES];
	int32_t current[AXES];
	int32_t next[AXES];
	int32_t from[AXES];
	struct line_data data;
	bool active;
	int32_t *vars[AXES];
} linestate_t;

bool do_line(linestate_t *state);
