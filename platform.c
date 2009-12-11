#include "platform.h"

#include <avr/io.h>

#ifdef __AVR_ATmega644P__
const portid_t pin_portid[] =
{PID_B, PID_B, PID_B, PID_B, PID_B, PID_B, PID_B, PID_B,  /* 1-8 */
 0, 0, 0, 0, 0,											  /* 9-13 */
 PID_D, PID_D, PID_D, PID_D, PID_D, PID_D, PID_D, PID_D,  /* 14-21 */
 PID_C, PID_C, PID_C, PID_C, PID_C, PID_C, PID_C, PID_C,  /* 22-29 */
 0, 0, 0,												  /* 30-32 */
 PID_A, PID_A, PID_A, PID_A, PID_A, PID_A, PID_A, PID_A}; /* 33-40 */

volatile uint8_t * const pin_ddr[] =
{&DDRB, &DDRB, &DDRB, &DDRB, &DDRB, &DDRB, &DDRB, &DDRB,  /* 1-8 */
 0, 0, 0, 0, 0,											  /* 9-13 */
 &DDRD, &DDRD, &DDRD, &DDRD, &DDRD, &DDRD, &DDRD, &DDRD,  /* 14-21 */
 &DDRC, &DDRC, &DDRC, &DDRC, &DDRC, &DDRC, &DDRC, &DDRC,  /* 22-29 */
 0, 0, 0,												  /* 30-32 */
 &DDRA, &DDRA, &DDRA, &DDRA, &DDRA, &DDRA, &DDRA, &DDRA}; /* 33-40 */

volatile uint8_t * const pin_port[] =
{&PORTB, &PORTB, &PORTB, &PORTB, &PORTB, &PORTB, &PORTB, &PORTB, /* 1-8 */
 0, 0, 0, 0, 0,					/* 9-13 */
 &PORTD, &PORTD, &PORTD, &PORTD, &PORTD, &PORTD, &PORTD, &PORTD, /* 14-21 */
 &PORTC, &PORTC, &PORTC, &PORTC, &PORTC, &PORTC, &PORTC, &PORTC, /* 22-29 */
 0, 0, 0,						/* 30-32 */
 &PORTA, &PORTA, &PORTA, &PORTA, &PORTA, &PORTA, &PORTA, &PORTA}; /* 33-40 */

volatile uint8_t * const pin_in[] =
{&PINB, &PINB, &PINB, &PINB, &PINB, &PINB, &PINB, &PINB, /* 1-8 */
 0, 0, 0, 0, 0,					/* 9-13 */
 &PIND, &PIND, &PIND, &PIND, &PIND, &PIND, &PIND, &PIND, /* 14-21 */
 &PINC, &PINC, &PINC, &PINC, &PINC, &PINC, &PINC, &PINC, /* 22-29 */
 0, 0, 0,						/* 30-32 */
 &PINA, &PINA, &PINA, &PINA, &PINA, &PINA, &PINA, &PINA}; /* 33-40 */

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
