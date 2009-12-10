#include "analog.h"

#include <avr/io.h>

void adc_init() {
	/* Enable ADC at sample rate (system clock)/8 */
	ADCSRA |= BV(ADEN) | BV(ADPS0) | BV(ADPS1);
}

uint16_t adc_sample(uint8_t channel){
	if(channel > 7) return 0;
	uint8_t low_byte, high_byte;
	ADMUX = channel;
	ADCSRA |= (1 << ADSC);
	/* Block until the ADC sampling is complete */
	while((ADCSRA & (1 << ADIF))  == 0){}; 
	low_byte = ADCL;
	high_byte = ADCH;
	ADCSRA |= (1 << ADIF);
	
	/* Merge high/low bytes.*/
	return (((uint16_t) high_byte) << 8) + low_byte;
}
