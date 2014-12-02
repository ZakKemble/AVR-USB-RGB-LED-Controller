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
#ifdef _WIN32
#include "win.h"
#endif
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

		for(uint32_t i=0;i<rgbLeds->size();i++)
		{
			// Generate random colour
			colour_t colour;
			colour.r = rand();
			colour.g = rand();
			colour.b = rand();
			
			// Gamma correction
			colour.r = (uint8_t)(pow(colour.r / 255.0, GAMMA_CORRECTION) * 255);
			colour.g = (uint8_t)(pow(colour.g / 255.0, GAMMA_CORRECTION) * 255);
			colour.b = (uint8_t)(pow(colour.b / 255.0, GAMMA_CORRECTION) * 255);

			// Send colour
			if(!rgbLeds->get(i)->setRGB(colour))
			{
				// If failed then remove controller from collection
				rgbLeds->erase(i);
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
