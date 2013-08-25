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

byte remap(byte, byte, byte, byte, byte);
byte limitVal(byte, byte, byte);

#endif /* UTIL_H_ */
