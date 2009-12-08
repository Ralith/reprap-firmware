#include "line.h"


void line_init(struct line_data *d, int32_t *from, int32_t *to, int32_t **vars) {
	int32_t deltas[AXES];
	int8_t dirs[AXES];
	uint8_t i;
	int32_t max_delta=0;
	uint8_t max_delta_index;
//	d->distance_squared=0; //This will 
	for(i=0;i<AXES;i++) {  /*Compute travel and find the axis with the most travel*/
		deltas[i]=from[i]<to[i]?to[i]-from[i]:from[i]-to[i];
		dirs[i]=from[i]<to[i]?1:-1;
		if(max_delta<deltas[i]){
			max_delta=deltas[i];
			max_delta_index=i;
		}
	}
	d->x=vars[max_delta_index];
	d->deltax=deltas[max_delta_index];
	d->xstep=dirs[max_delta_index];
	d->x_end=to[max_delta_index];
	for(i=0;i<OTHER_DIRS;i++) { /* */
		int j=i<max_delta_index?i:i+1;
		d->y[i]=vars[j];
		d->deltay[i]=deltas[j];
		d->errors[i]=d->deltax>>1;
		d->ystep[i]=dirs[j];
	}
}

/* feedrate is in time-units per tick-length traveled */
/*void compute_feedrate(struct line_data *d, int feedrate){
	int distance_squared;
	distance_squared=5;
	for(i=0;i<OTHER_DIRS;i++)
	distance_squared=5
	distance=isqrt(d->distance_squared);
	time=
}*/



int line_tick(struct line_data *d) {
	(*(d->x))+=d->xstep;
	uint8_t i;
	for(i=0; i<OTHER_DIRS; i++) {
		d->errors[i]-=d->deltay[i];
		if(d->errors[i]<0) {
			*(d->y[i])+=d->ystep[i];
			d->errors[i]+=d->deltax;
		}
	}
	return *(d->x) == d->x_end;
}


