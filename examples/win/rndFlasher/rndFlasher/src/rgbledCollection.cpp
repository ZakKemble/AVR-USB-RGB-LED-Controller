/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#include <iostream>
#include <vector>
#include <rgbled.h>
#include "rgbledCollection.h"

using namespace std;

rgbledCollection::rgbledCollection()
{

}

rgbledCollection::~rgbledCollection()
{
	clear();
}

// Remove all RGB LEDs
void rgbledCollection::clear()
{
	while(!rgbLeds.empty())
		delete rgbLeds.back();
	rgbLeds.clear();
}

// Add an RGB LED
void rgbledCollection::add(rgbled* rgbLed)
{
	rgbLeds.push_back(rgbLed);
}

// Remove an RGB LED from list
void rgbledCollection::remove(uint32_t idx)
{
	rgbLeds.erase(rgbLeds.begin() + idx);
}

// Remove an RGB LED from list then delete the object
void rgbledCollection::erase(uint32_t idx)
{
	rgbled* rgbLed = get(idx);
	remove(idx);
	delete rgbLed;
}

// Get an RGB LED
rgbled* rgbledCollection::get(uint32_t idx)
{
	return rgbLeds.at(idx);
}

// Get number of RGB LEDs
uint32_t rgbledCollection::size()
{
	return rgbLeds.size();
}

// Go through RGB LEDs and see if one of the USB handles is a handle to the same device as the handle for the passed RGB LED
bool rgbledCollection::has(rgbled* rgbLed)
{
	for(uint32_t i=0;i<size();i++)
	{
		if(rgbled::sameDevice(rgbLeds.at(i), rgbLed))
			return true;
	}
	return false;
}

// Print info about an RGB LED
void rgbledCollection::print(uint32_t idx)
{
	cout << "------" << endl;
	if(idx >= rgbLeds.size())
	{
		cout << "Bad ID: " << idx << endl;
		return;
	}

	cout << "Device number: " << idx << endl;

	rgbled* rgbLed = rgbLeds.at(idx);

	//
	rgbled_version_t version;
	rgbLed->getVersion(version);
	cout << "\t" << "Firmware version: " << (int)version.major << "." << (int)version.minor << endl;

	//
	cout << "\t" << "EEPROM size: " << rgbLed->getEepromSize() << endl;

	//
	colour_t colour;
	rgbLed->getColour(colour);
	cout << "\t" << "Current colour: " << (int)colour.r << " " << (int)colour.g << " " << (int)colour.b << endl;
}
