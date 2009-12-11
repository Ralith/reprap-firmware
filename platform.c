#include "platform.h"

#include <avr/io.h>

#ifdef __AVR_ATmega644P__
const portid_t pin_pid[] =
{PID_B, PID_B, PID_B, PID_B, PID_B, PID_B, PID_B, PID_B,  /* 1-8 */
 0, 0, 0, 0, 0,											  /* 9-13 */
 PID_D, PID_D, PID_D, PID_D, PID_D, PID_D, PID_D, PID_D,  /* 14-21 */
 PID_C, PID_C, PID_C, PID_C, PID_C, PID_C, PID_C, PID_C,  /* 22-29 */
 0, 0, 0,												  /* 30-32 */
 PID_A, PID_A, PID_A, PID_A, PID_A, PID_A, PID_A, PID_A}; /* 33-40 */

volatile uint8_t * const pid_ddr[] = {0, &DDRA, &DDRB, &DDRC, &DDRD};
volatile uint8_t * const pid_port[] = {0, &PORTA, &PORTB, &PORTC, &PORTD};
volatile uint8_t * const pid_in[] = {0, &PINA, &PINB, &PINC, &PIND};

const int8_t pin_offset[] =
{0, 1, 2, 3, 4, 5, 6, 7,		/* 1-8 */
 -1, -1, -1, -1, -1,			/* 9-13 */
 0, 1, 2, 3, 4, 5, 6, 7,		/* 14-21 */
 0, 1, 2, 3, 4, 5, 6, 7,		/* 22-29 */
 -1, -1, -1,					/* 30-32 */
 7, 6, 5, 4, 3, 2, 1, 0};		/* 33-40 */
#else
#error The target platform is unsupported.
#endif
