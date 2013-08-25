/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include <stdbool.h>

typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef uint16_t eepAddr_t;

typedef struct{
	byte red;
	byte green;
	byte blue;
}s_rgbVal;

typedef struct{
	byte idleTime;
}s_settings;

typedef struct{
	bool enabled;
	uint counter;
}s_timer;

#endif /* TYPEDEFS_H_ */