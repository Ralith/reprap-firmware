#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "digital.h"
#include "uart.h"
#include "gcode.h"
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
		/* TODO: Handle UART errors */
		if(recv != UART_NO_DATA) {
			static int8_t result;
			result = gcode_parsec(recv);
			switch(result) {
			case GCODE_BLOCK_COMPLETE:
				/* Indicate that we're ready for the next block. */
				/* TODO: Use real hardware or software flow control in addition. */
				uart_puts_P("ok\r\n");
				break;

			case GCODE_INVALID_WORD:
				uart_puts_P("WARNING: INVALID GCODE WORD\r\n");
				break;

			default:
				if(result < 0) {
					uart_puts_P("WARNING: ERROR DURING PARSE\r\n");
				}
				break;
			}
		}
	}
	
	return 0;
}
