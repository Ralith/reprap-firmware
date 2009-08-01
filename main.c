#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "digital.h"
#include "uart.h"

#include "stepdrive.h"

#define UART_BAUD_RATE 19200

int main(void)
{
	/* Initialize stepper control */
	stepdrive_init();
		
	/* Enable UART for serial comms */
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));
	/* Enable interrupts needed by UART lib */
	sei();

	unsigned short recv;
	for(;; recv = uart_getc()) {
		/* TODO: Handle errors */
		if(recv != UART_NO_DATA) {
			gcode_parsec(recv);
		}
	}
	
	return 0;
}
