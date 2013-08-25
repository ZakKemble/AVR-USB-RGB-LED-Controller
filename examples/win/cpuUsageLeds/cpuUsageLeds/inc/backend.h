/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#ifndef BACKEND_H_
#define BACKEND_H_

#include "typedefs.h"

void backend(void);
void backend_close(void);
//void backend_wait(void);
void backend_deviceReset(void);
void backend_setMode(byte);
void backend_setColour(byte, byte, byte);
void backend_setTransitionTime(uint);
void backend_setBrightness(byte);
void backend_setIdleTime(byte);

#endif /* BACKEND_H_ */
