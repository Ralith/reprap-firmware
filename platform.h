#include "types.h"

/* 0 if not an addressable pin */
extern const portid_t pin_pid[];
extern volatile uint8_t * const pid_ddr[];
extern volatile uint8_t * const pid_port[];
extern volatile uint8_t * const pid_in[];
/* -1 if not an addressable pin */
extern const int8_t pin_offset[];
