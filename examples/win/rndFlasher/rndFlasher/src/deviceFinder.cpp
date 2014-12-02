/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#include <iostream>
#include <rgbled.h>
#include "deviceFinder.h"
#include "rgbledCollection.h"

using namespace std;

deviceFinder::deviceFinder()
{

}

deviceFinder::~deviceFinder()
{

}

uint32_t deviceFinder::getNewDevices(rgbledCollection* rgbLeds)
{
	uint32_t count = rgbled::find();
	cout << "Found " << count << " devices" << endl;

	uint32_t newCount = 0;

	for(uint32_t i=0;i<count;i++)
	{
		rgbled* rgbLed = new rgbled();
		if(rgbLed->open(i) && !rgbLeds->has(rgbLed))
		{
			newCount++;
			cout << "New RGB LED controller found!" << endl;
			rgbLeds->add(rgbLed);
			rgbLeds->print(rgbLeds->size() - 1);
		}
		else
			delete rgbLed; // This also closes the device
	}
	
	cout << "Added " << newCount << " new devices" << endl;

	return newCount;
}
