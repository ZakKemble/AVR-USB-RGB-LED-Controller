/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef uint16_t eepAddr_t;

typedef struct{
	byte r;
	byte g;
	byte b;
}colour_t;

typedef struct{
	byte idleTime;
}settings_t;

typedef struct{
	bool enabled;
	uint counter;
}timer_t;

#endif /* TYPEDEFS_H_ */