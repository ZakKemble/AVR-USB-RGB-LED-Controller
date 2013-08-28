/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#ifndef FLASHER_H_
#define FLASHER_H_

#include "rgbledCollection.h"

class flasher
{
public:
	flasher();
	~flasher();
	void begin();

private:
	rgbledCollection* rgbLeds;
};

#endif /* FLASHER_H_ */
