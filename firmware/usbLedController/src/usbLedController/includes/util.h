/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#ifndef UTIL_H_
#define UTIL_H_

#include "typedefs.h"
#include <avr/sfr_defs.h>

#define delay(ms) _delay_ms(ms)
#define delay_us(us) _delay_us(us)

#define CONCAT(a, b) a ## b

#define B0	B, 0
#define B1	B, 1
#define B2	B, 2
#define B3	B, 3
#define B4	B, 4
#define B5	B, 5

#define bit_set_concat(reg, reg2, bit)		(CONCAT(reg, reg2) |= _BV(bit))
#define bit_clr_concat(reg, reg2, bit)		(CONCAT(reg, reg2) &= ~_BV(bit))
#define bit_tog_concat(reg, reg2, bit)		(CONCAT(reg, reg2) ^= _BV(bit))
#define bit_is_set_concat(reg, reg2, bit)	(bit_is_set(CONCAT(reg, reg2), bit))

#define HIGH			bit_set_concat
#define LOW				bit_clr_concat
#define TOGGLE			bit_tog_concat
#define pinWrite(x, s)	s(PORT, x)

#define OUTPUT			bit_set_concat
#define INPUT			bit_clr_concat
#define pinMode(x, s)	s(DDR, x)

#define PULLUP_ENABLE	bit_set_concat
#define PULLUP_DISABLE	bit_clr_concat
#define pinPullup(x, s) s(PORT, x)

#define PINHIGH			true
#define PINLOW			false
#define pinRead(x)		bit_is_set_concat(PIN, x)

#define bits_set(data, bits)	((data) |= (bits))
#define bits_clr(data, bits)	((data) &= (~(bits)))
#define bit_set(data, x)		bits_set((data), _BV(x))
#define bit_clr(data, x)		bits_clr((data), _BV(x))

#endif /* UTIL_H_ */
