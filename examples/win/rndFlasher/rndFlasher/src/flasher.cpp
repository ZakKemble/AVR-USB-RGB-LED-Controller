/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "flasher.h"
#include "rgbledCollection.h"
#include "deviceFinder.h"

#ifndef _WIN32
#include <unistd.h>
#endif

#define GAMMA_CORRECTION 2.2

using namespace std;

flasher::flasher()
{
	// Create RGB LED collection
	rgbLeds = new rgbledCollection();
}

flasher::~flasher()
{
	delete rgbLeds;
}

void flasher::begin()
{
	while(1)
	{
		// Find any new controllers and add to collection
		deviceFinder::getNewDevices(rgbLeds);

		for(uint i=0;i<rgbLeds->size();i++)
		{
			s_rgbVal colour;
			colour.red = rand() % 2;
			colour.green = rand() % 2;
			colour.blue = rand() % 2;

			colour.red *= 255;
			colour.green *= 255;
			colour.blue *= 255;
			
			//colour.red = (byte)(pow(colour.red / 255.0, GAMMA_CORRECTION) * 255);
			//colour.green = (byte)(pow(colour.green / 255.0, GAMMA_CORRECTION) * 255);
			//colour.blue = (byte)(pow(colour.blue / 255.0, GAMMA_CORRECTION) * 255);

			// Send colour
			if(!rgbLeds->get(i)->setRGB(colour))
			{
				// If failed then remove controller from collection
				rgbledctrl* rgbLed = rgbLeds->get(i);
				rgbLeds->remove(i);
				delete rgbLed;
				cout << "Device removed" << endl;
			}
		}

#ifdef WIN32
		Sleep(200);
#else
		usleep(200 * 1000);
#endif
	}
}
