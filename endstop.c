#include "endstop.h"

#include <avr/interrupt.h>

#include "util.h"
#include "platform.h"
#include "config.h"
#include "digital.h"

digstate_t endstops[2*AXES]; /* [xmin|xmax|ymin|ymax|zmin|zmax|...] */

void endstop_enable_interrupt(pin_t pin) {
	switch(pin_pid[pin]) {
	case PID_A:
		BSET(PCICR, PCIE0, 1);
		BSET(PCMSK0, pin_offset[pin], 1);
		break;
		
	case PID_B:
		BSET(PCICR, PCIE1, 1);
		BSET(PCMSK1, pin_offset[pin], 1);
		break;
		
	case PID_C:
		BSET(PCICR, PCIE2, 1);
		BSET(PCMSK2, pin_offset[pin], 1);
		break;
		
	case PID_D:
		BSET(PCICR, PCIE3, 1);
		BSET(PCMSK3, pin_offset[pin], 1);
		break;

	default:
		/* TODO: Indicate error */
		break;
	}
}

void endstop_init() {
	uint8_t i;

	/* Initialize state array */
	for(i = 0; i < 2*AXES; i++) {
		endstops[i] = ENDSTOP_UNDEFINED;
	}

	/* Enable state change interrupts */
	for(i = 0; i < AXES; i++) {
		if(MIN_PIN[i]) {
			dig_mode(MIN_PIN[i], INPUT);
			endstop_enable_interrupt(MIN_PIN[i]);
		}
		if(MAX_PIN[i]) {
			dig_mode(MAX_PIN[i], INPUT);
			endstop_enable_interrupt(MAX_PIN[i]);
		}
	}
}

/* Pin state change */
#define SET_ENDSTOP(axis, index, state) endstops[2*axis+index] = state
ISR(PCINT0_vect) 
{
	uint8_t i;
	for(i = 0; i < AXES; i++) {
		if(MIN_PIN[i]) {
			SET_ENDSTOP(i, 0, dig_read(MIN_PIN[i]));
		}
		if(MAX_PIN[i]) {
			SET_ENDSTOP(i, 1, dig_read(MAX_PIN[i]));
		}
	}
}

/* Bind all pin change interrupts to the above function */
ISR(PCINT1_vect, ISR_ALIASOF(PCINT0_vect));
ISR(PCINT2_vect, ISR_ALIASOF(PCINT0_vect));
ISR(PCINT3_vect, ISR_ALIASOF(PCINT0_vect));
