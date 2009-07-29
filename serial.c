#include "serial.h"

#include <avr/io.h>

#include "util.h"

#define TIMING ((F_CPU / 16 + BAUD / 2) / BAUD - 1)

void ser_init() 
{
	/* Set baud */
	UBRR0H = TIMING >> 8;
	UBRR0L = TIMING;

	/* Set framing to 8N1 */
	UCSR0C = (3 << UCSZ00);

	/* Enable UART */
	UCSR0B = BV(RXEN0) | BV(TXEN0);
}

void ser_write(ubyte b) 
{
	/* Wait until ready? */
	while(!(UCSR0A & BV(UDRE0))) {}

	UDR0 = b;
}

void ser_write_str(char *buf)
{
	while(*buf != '\0') {
		ser_write(*buf);
		buf++;
	}
}

