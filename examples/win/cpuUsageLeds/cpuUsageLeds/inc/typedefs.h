/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include "win.h"
#include <stdbool.h>
#include <stdint.h>
#include <rgbled.h>

typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;

typedef struct{
	byte brightness;
	byte ledMode;
	uint16_t transitionTime;
	rgbled_t* rgbled_device;
}s_device;

// This extra visible var is because when a window is shown
// its visible status is not set until AFTER the WM_SHOWWINDOW message
typedef struct{
	HWND dlg;
	bool visible;
}s_dlg;

#endif /* TYPEDEFS_H_ */
