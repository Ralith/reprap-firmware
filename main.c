#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "digital.h"
#include "uart.h"

#define UART_BAUD_RATE 19200

int main(void)
{
	/* Enable UART for serial comms */
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));
	/* Enable interrupts needed by UART lib */
	sei();

	dig_mode(1, OUTPUT);
	dig_mode(19, INPUT);

	digstate_t state;
	while(1) {
		state = dig_read(19);
		dig_write(1, state);
		if(state == LOW) {
			uart_puts("LOW\r\n");
		} else {
			uart_puts("HIGH\r\n");
		}
	}
	return 0;
}
