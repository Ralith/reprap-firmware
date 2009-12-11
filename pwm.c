#include <avr/io.h>
#include <avr/interrupt.h>

#include "util.h"

bool pwm_init(pin_t pin) {
	/* TODO: Other timer support, portability */
	uint8_t pin_mask;
	if(pin == 4) {
		mask = BV(COM0A1);
	} else if(pin == 5) {
		mask = BV(COM0B1);
	}
	
	/* TODO: Other pins */
	/* Note: Uses clock 1 */
	/* Fast PWM mode, invert pin when at 0xFF */
	TCCR0A |= pin_mask | BV(WGM01) | BV(WGM00);
	/* Run at clock/64 */
	TCCR0B |= BV(CS01) | BV(CS00);
}

void pwm_set(pin_t pin, uint8_t duty_cycle) {
	/* TODO: Other timer support, portability */
	if(pin == 4) {
		OCR0A = duty_cycle;
	} else if(pin == 5) {
		OCR0B = duty_cycle;
	}
}
