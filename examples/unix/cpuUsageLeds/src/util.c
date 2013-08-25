/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#include "typedefs.h"
#include "util.h"

byte remap(byte x, byte in_min, byte in_max, byte out_min, byte out_max)
{
	return (((int)x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

byte limitVal(byte val, byte min, byte max)
{
	if(val < min)
		return min;
	else if(val > max)
		return max;
	else
		return val;
}
