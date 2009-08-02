#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "digital.h"
#include "uart.h"
#include "stepdrive.h"
#include "config.h"

#define UART_BAUD_RATE 19200

int main(void)
{
	/* Initialize stepper control */
	stepdrive_init();
	/* Enable UART for serial comms */
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));
	/* Enable interrupts */
	sei();

	/* Indicate ready state to host software */
	/* TODO: Use real hardware or software flow control */
	uart_puts_P("start");
#ifdef DEBUG_LED_PIN
	dig_mode(DEBUG_LED_PIN, OUTPUT);
	dig_write(DEBUG_LED_PIN, HIGH);
#endif
	uint16_t recv;
	for(;; recv = uart_getc()) {
		/* TODO: Handle errors */
		if(recv != UART_NO_DATA) {
			gcode_parsec(recv);
		}
	}
	
	return 0;
}
