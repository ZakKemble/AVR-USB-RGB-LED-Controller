/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#ifndef DEVICEFINDER_H_
#define DEVICEFINDER_H_

#include "rgbledCollection.h"

class deviceFinder
{
public:
	deviceFinder();
	~deviceFinder();
	static uint getNewDevices(rgbledCollection*);

private:

};

#endif /* DEVICEFINDER_H_ */
