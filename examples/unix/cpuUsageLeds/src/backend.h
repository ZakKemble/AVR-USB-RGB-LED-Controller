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

void backend(byte, ushort);
void backend_close(void);

#endif /* BACKEND_H_ */
