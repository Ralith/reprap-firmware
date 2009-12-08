#include "types.h"

#define OTHER_DIRS (AXES-1)

struct line_data {
	int32_t *x;
	int32_t *y[OTHER_DIRS];
	int32_t deltax;
	int32_t xstep;
	int32_t x_end;
	int32_t deltay[OTHER_DIRS];
	int32_t errors[OTHER_DIRS];
	int32_t ystep[OTHER_DIRS];
	int32_t distance_squared;
};

void line_init(struct line_data *data, int32_t *from, int32_t *to, int32_t **vars);
int line_tick(struct line_data *data);
