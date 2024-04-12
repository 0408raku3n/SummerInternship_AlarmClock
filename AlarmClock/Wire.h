/*
 * Wire.h - TWI/I2C library for Atmel CPU.
 */

#include <avr/io.h>
#ifdef TWDR

#include "TwoWire.h"

#else

#ifdef USIDR

#include "USIWire.h"

#else

#if defined(TWSD) && !defined(__AVR_ATtiny1634__)

#include "TWSWire.h"

#else

#error No Wire support on unknown board

#endif // TWSD
#endif // USIDR
#endif // TWDR
