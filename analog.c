#include "analog.h"

#include <avr/io.h>

void adc_init() {
	/* Enable ADC at sample rate (system clock)/8 */
	ADCSRA |= (1 << ADEN) | (1 << ADPS0 ) | (1 << ADPS1 );
}
