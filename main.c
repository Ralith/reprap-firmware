#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "digital.h"
#include "uart.h"
#include "gcode.h"
#include "stepdrive.h"
#include "extruder.h"
#include "config.h"

#define UART_BAUD_RATE 19200

#ifdef DEBUG_LED_PIN
#define ERROR(msg) uart_puts_P("ERROR: " msg "\r\n"); dig_write(DEBUG_LED_PIN, LOW);
#else
#define ERROR(msg) uart_puts_P("ERROR: " msg "\r\n");
#endif
	

int main(void)
{
	/* Initialize hardware control */
	stepdrive_init();
	extruder_init();
	/* Enable UART for serial comms */
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));
	/* Enable interrupts */
	sei();

#ifdef DEBUG_LED_PIN
	dig_mode(DEBUG_LED_PIN, OUTPUT);
	dig_write(DEBUG_LED_PIN, HIGH);
#endif

	/* Indicate ready state to host software */
	/* TODO: Use real hardware or software flow control */
	uart_puts_P("start\r\n");
	uint16_t recv;
	while(1) {
		recv = uart_getc();
		switch(recv >> 8) {
		case UART_FRAME_ERROR >> 8:
			ERROR("FRAME ERROR. Bad transmission or busy controller");
			break;
			
		case UART_OVERRUN_ERROR >> 8:
			ERROR("OVERRUN. Receiving data too quickly");
			break;
			
		case UART_BUFFER_OVERFLOW >> 8:
			ERROR("BUFFER OVERFLOW. Not parsing fast enough");
			break;

		case UART_NO_DATA >> 8:
			break;

		default:
		{
			/* Got data */
			static int8_t result;
			result = gcode_parsec((char)recv);
			switch(result) {
			case GCODE_BLOCK_COMPLETE:
				/* Indicate that we're ready for the next block. */
				/* TODO: Use real hardware or software flow control in addition. */
				uart_puts_P("ok\r\n");
				break;

			case GCODE_INVALID_WORD:
				ERROR("INVALID GCODE WORD");
				break;

			default:
				if(result < 0) {
					ERROR("ERROR DURING PARSE");
				}
				break;
			}
		}
		}
	}
	
	return 0;
}
