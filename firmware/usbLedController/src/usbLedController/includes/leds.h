/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#ifndef LEDS_H_
#define LEDS_H_

void leds_init(void);
colour_t* leds_colour(void);
void leds_apply(void);
void leds_setColour(byte, byte, byte);

#endif /* LEDS_H_ */