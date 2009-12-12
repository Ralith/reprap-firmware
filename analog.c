#include "analog.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#include "util.h"

void (*_adc_callback)(const uint16_t) = 0;

bool adc_init(const pin_t pin, void (*callback)(const uint16_t)) {
	/* Verify and select input pin */
	const int8_t mux = 40 - pin;
	if(mux < 0) {
		return FALSE;
	}
	ADMUX = mux;

	_adc_callback = callback;

	/* Set free-running mode */
	ADCSRB = 0;
	
	/* Enable interrupting ADC at sample rate clock/128 */
	ADCSRA |= BV(ADEN)
		| BV(ADIE)
		| BV(ADPS2) | BV(ADPS1) | BV(ADPS0)
		| BV(ADSC);

	return TRUE;
}

ISR(ADC_vect) {
	_adc_callback((((uint16_t) ADCH) << 8) + ADCL);
}
