#include "motion.h"

#include "config.h"
#include "gcode.h"
#include "digital.h"

bool do_line(linestate_t *state) {
	/* TODO: Move this state to caller to avoid glitching on endstop hit. */
	uint8_t i;

	if(!state->active) {
		state->active = TRUE;
		
		for(i=0;i<AXES;i++) { /* Not sure if memcpy will unroll off the top of my head
							   * Written as a loop to allow more axes
							   * Some other stuff to do anyways. */
			state->from[i]=state->current[i];
			state->vars[i]=&(state->next[i]);
		}

		for(i = 0; i < AXES; i++) {
			state->to[i] = instructions[inst_read].position[i];
		}

		/* Prepping the line should probably be done asynchronous to the tick timer -
		 * perhaps check need_inst at the end of the timer, and switch between two
		 * line_data structs, so that we prep the next line while running the previous one. */
		line_init(&state->data, state->from, state->to, state->vars);
		/* TODO: Set the timer such that a motor stepping every tick will move at a sane
		 * maximum rate. */
		/* TODO: Calculate feedrate and set the timer appropriately. */	
	}

	state->active = line_tick(&state->data);
	for(i = 0; i < AXES; i++) {
		/* This seems awkward, but I'm not sure that it'd be any better
		 * making line_tick directly return step and dir, given that it
		 * needs to track the position anyways. */
		int32_t tick = state->current[i]-state->next[i];
		if(tick != 0) {
			if(INVERT_AXIS[i]) {
				dig_write(DIR_PIN[i], (tick > 0 ? HIGH : LOW));
			} else {
				dig_write(DIR_PIN[i], (tick > 0 ? LOW : HIGH));
			}
			dig_write(STEP_PIN[i], HIGH);
			/* TODO: Verify that we don't need to sleep here. */
			dig_write(STEP_PIN[i], LOW);
		}
		/* ALTERNATE: If directly controlling steppers,
		 * calculate the next configuration of coils directly with mod and bitshift. */
		state->current[i]=state->next[i];
	}

	return state->active;
}
