/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#include <iostream>
#include <rgbledctrl.h>
#include "deviceFinder.h"
#include "rgbledCollection.h"

using namespace std;

deviceFinder::deviceFinder()
{

}

deviceFinder::~deviceFinder()
{

}

uint deviceFinder::getNewDevices(rgbledCollection* rgbLeds)
{
	uint count = rgbledctrl::find();
	cout << "Found " << count << " devices" << endl;

	uint newCount = 0;

	for(uint i=0;i<count;i++)
	{
		rgbledctrl* rgbLed = new rgbledctrl();
		if(rgbLed->open(i) && !rgbLeds->has(rgbLed))
		{
			newCount++;
			cout << "New RGB LED controller found!" << endl;
			rgbLeds->add(rgbLed);
			rgbLeds->print(rgbLeds->size() - 1);
		}
		else
			delete rgbLed;
	}
	
	cout << "Added " << newCount << " new devices" << endl;

	return newCount;
}
