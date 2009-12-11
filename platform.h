#include "types.h"

/* 0 if not an addressable pin */
extern const portid_t pin_portid[];
extern volatile uint8_t * const pin_ddr[];
extern volatile uint8_t * const pin_port[];
extern volatile uint8_t * const pin_in[];
/* -1 if not an addressable pin */
extern const int8_t pin_offset[];
