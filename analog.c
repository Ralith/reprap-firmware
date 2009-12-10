#include "analog.h"

#include <avr/io.h>

void adc_init() {
	/* Enable ADC at sample rate (system clock)/8 */
	ADCSRA |= BV(ADEN) | BV(ADPS0) | BV(ADPS1);
}
