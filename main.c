#include <avr/io.h>
#include <util/delay.h>

#include "digital.h"
#include "serial.h"

int main(void)
{
	ser_init();
	dig_mode(1, OUTPUT);
	dig_mode(19, INPUT);

	digstate_t state;
	while(1) {
		state = dig_read(19);
		dig_write(1, state);
		if(state == LOW) {
			ser_write_str("LOW\r\n");
		} else {
			ser_write_str("HIGH\r\n");
		}
	}
	return 0;
}
