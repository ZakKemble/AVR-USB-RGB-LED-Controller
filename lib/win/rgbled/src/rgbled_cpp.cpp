/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#include <string.h>
#include "rgbled.h"

#ifdef _WIN32
	#define LIB_EXPORT __declspec(dllexport)
#else
	#define LIB_EXPORT
#endif

// CPP wrapper


//
// Static helper methods
//

void LIB_EXPORT rgbled::init()
{
	rgbled_init();
}

void LIB_EXPORT rgbled::exit()
{
	rgbled_exit();
}

uint32_t LIB_EXPORT rgbled::find()
{
	return rgbled_find();
}

bool LIB_EXPORT rgbled::sameDevice(rgbled* dev1, rgbled* dev2)
{
	return rgbled_sameDevice(dev1->getDevice(), dev2->getDevice());
}






//
// Main class stuff
//

LIB_EXPORT rgbled::rgbled()
{
	device = NULL;
}

LIB_EXPORT rgbled::~rgbled()
{
	close();
}

bool LIB_EXPORT rgbled::open()
{
	device = rgbled_open();
	return device != NULL;
}

bool LIB_EXPORT rgbled::open(uint32_t idx)
{
	device = rgbled_open_byIndex(idx);
	return device != NULL;
}

bool LIB_EXPORT rgbled::open(uint8_t value, eepAddr_t address)
{
	device = rgbled_open_byEEPROM(value, address);
	return device != NULL;
}

void LIB_EXPORT rgbled::close()
{
	rgbled_close(device);
	device = NULL;
}

bool LIB_EXPORT rgbled::poke()
{
	return rgbled_poke(device);
}

bool LIB_EXPORT rgbled::reset()
{
	return rgbled_reset(device);
}

bool LIB_EXPORT rgbled::setIdleTime(uint8_t time)
{
	return rgbled_setIdleTime(device, time);
}

bool LIB_EXPORT rgbled::setRGB(colour_t& colour)
{
	return rgbled_setRGB(device, &colour);
}

bool LIB_EXPORT rgbled::setR(uint8_t value)
{
	return rgbled_setR(device, value);
}

bool LIB_EXPORT rgbled::setG(uint8_t value)
{
	return rgbled_setG(device, value);
}

bool LIB_EXPORT rgbled::setB(uint8_t value)
{
	return rgbled_setB(device, value);
}

bool LIB_EXPORT rgbled::eeprom_write(uint8_t data, eepAddr_t address)
{
	return rgbled_eeprom_write(device, data, address);
}

bool LIB_EXPORT rgbled::eeprom_read(uint8_t& data, eepAddr_t address)
{
	return rgbled_eeprom_read(device, &data, address);
}



//
// Getters
//

void LIB_EXPORT rgbled::getVersion(rgbled_version_t& version)
{
	memcpy(&version, &device->version, sizeof(version));
}

eepAddr_t LIB_EXPORT rgbled::getEepromSize()
{
	return device->eepromSize;
}

void LIB_EXPORT rgbled::getColour(colour_t& colour)
{
	memcpy(&colour, &device->colour, sizeof(colour_t));
}

uint8_t LIB_EXPORT rgbled::getIdleTime()
{
	return device->settings.idleTime;
}

void* LIB_EXPORT rgbled::getHandle()
{
	return device->handle;
}

// What about path?

rgbled_t* LIB_EXPORT rgbled::getDevice()
{
	return device;
}
