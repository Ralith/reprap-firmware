#include "motion.h"

#include "gcode.h"
#include "line.h"
#include "digital.h"

bool do_line() {
	static int32_t to[AXES];
	static int32_t current[AXES];
	static int32_t next[AXES];
	static int32_t from[AXES];
	static struct line_data this_line;
	static bool in_line = FALSE;
	static int32_t *vars[AXES];

	uint8_t i;

	if(!in_line) {
		in_line = TRUE;
		
		for(i=0;i<AXES;i++) { /* Not sure if memcpy will unroll off the top of my head
							   * Written as a loop to allow more axes
							   * Some other stuff to do anyways. */
			from[i]=current[i];
			vars[i]=&(next[i]);
		}

		for(i = 0; i < AXES; i++) {
			to[i] = instructions[inst_read].position[i];
		}

		/* Prepping the line should probably be done asynchronous to the tick timer -
		 * perhaps check need_inst at the end of the timer, and switch between two
		 * line_data structs, so that we prep the next line while running the previous one. */
		line_init(&this_line, from, to, vars);
		/* TODO: Set the timer such that a motor stepping every tick will move at a sane
		 * maximum rate. */
		/* TODO: Calculate feedrate and set the timer appropriately. */	
	}

	in_line = line_tick(&this_line);
	for(i = 0; i < AXES; i++) {
		/* This seems awkward, but I'm not sure that it'd be any better
		 * making line_tick directly return step and dir, given that it
		 * needs to track the position anyways. */
		int32_t tick = current[i]-next[i];
		if(tick != 0) {
			if(INVERT_AXIS[i]) {
				dig_write(DIR_PIN[i], (tick > 0 ? HIGH : LOW));
			} else {
				dig_write(DIR_PIN[i], (tick > 0 ? LOW : HIGH));
			}
			dig_write(STEP_PIN[i], HIGH);
			dig_write(STEP_PIN[i], LOW);
		}
		/* ALTERNATE: If directly controlling steppers,
		 * calculate the next configuration of coils directly with mod and bitshift. */
		current[i]=next[i];
	}

	return in_line;
}
