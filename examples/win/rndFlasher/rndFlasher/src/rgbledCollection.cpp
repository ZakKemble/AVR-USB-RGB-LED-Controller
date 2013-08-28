/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#include <iostream>
#include <vector>
#include <rgbledctrl.h>
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
void rgbledCollection::add(rgbledctrl* rgbLed)
{
	rgbLeds.push_back(rgbLed);
}

// Remove an RGB LED
void rgbledCollection::remove(uint idx)
{
	rgbLeds.erase(rgbLeds.begin() + idx);
}

// Get an RGB LED
rgbledctrl* rgbledCollection::get(uint idx)
{
	return rgbLeds.at(idx);
}

// Get number of RGB LEDs
uint rgbledCollection::size()
{
	return rgbLeds.size();
}

// Go through RGB LEDs and see if one of the USB handles is a handle to the same device as the handle for the passed RGB LED
bool rgbledCollection::has(rgbledctrl* rgbLed)
{
	for(uint i=0;i<size();i++)
	{
		if(rgbledctrl::sameDevice(rgbLeds.at(i)->getHandle(), rgbLed->getHandle()))
			return true;
	}
	return false;
}

// Print info about an RGB LED
void rgbledCollection::print(uint idx)
{
	cout << "------" << endl;
	if(idx >= rgbLeds.size())
	{
		cout << "Bad ID: " << idx << endl;
		return;
	}

	cout << "Device number: " << idx << endl;

	rgbledctrl* rgbLed = rgbLeds.at(idx);

	//
	byte version[2];
	rgbLed->getVersion(version);
	cout << "\t" << "Firmware version: " << (int)version[0] << "." << (int)version[1] << endl;

	//
	cout << "\t" << "EEPROM size: " << rgbLed->getEepromSize() << endl;

	//
	s_rgbVal colour;
	rgbLed->getColour(colour);
	cout << "\t" << "Current colour: " << (int)colour.red << " " << (int)colour.green << " " << (int)colour.blue << endl;
}
