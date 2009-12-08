#include "types.h"

#define OTHER_DIRS (AXES-1)

struct line_data {
	int *x;
	int *y[OTHER_DIRS];
	int deltax;
	int xstep;
	int x_end;
	int deltay[OTHER_DIRS];
	int errors[OTHER_DIRS];
	int ystep[OTHER_DIRS];
	int distance_squared;
};

void line_init(struct line_data *data, int *from, int *to, int **vars);
int line_tick(struct line_data *data);
