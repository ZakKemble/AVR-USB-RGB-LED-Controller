/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#ifdef _WIN32
#include <lusb0_usb.h>
#else
#include <usb.h>
#endif
#include <string.h>
#include <rgbledctrl.h>

// CPP wrapper


//
// Static helper methods
//

void rgbledctrl::init()
{
	rgbledctrl_init();
}

uint rgbledctrl::find()
{
	return rgbledctrl_find();
}

bool rgbledctrl::sameDevice(usb_dev_handle* handle1, usb_dev_handle* handle2)
{
	return rgbledctrl_sameDevice(handle1, handle2);
}



//
// Main class stuff
//

rgbledctrl::rgbledctrl()
{
	device = NULL;
}

rgbledctrl::~rgbledctrl()
{
	close();
}

bool rgbledctrl::open()
{
	device = rgbledctrl_open();
	return device != NULL;
}

bool rgbledctrl::open(uint idx)
{
	device = rgbledctrl_open_byIndex(idx);
	return device != NULL;
}

bool rgbledctrl::open(byte value, eepAddr_t address)
{
	device = rgbledctrl_open_byEEPROM(value, address);
	return device != NULL;
}

void rgbledctrl::close()
{
	rgbledctrl_close(device);
	device = NULL;
}

bool rgbledctrl::poke()
{
	return rgbledctrl_poke(device);
}

bool rgbledctrl::reset()
{
	return rgbledctrl_reset(device);
}

bool rgbledctrl::setIdleTime(byte time)
{
	return rgbledctrl_setIdleTime(device, time);
}

bool rgbledctrl::setRGB(s_rgbVal& colour)
{
	return rgbledctrl_setRGB(device, &colour);
}

bool rgbledctrl::setR(byte value)
{
	return rgbledctrl_setR(device, value);
}

bool rgbledctrl::setG(byte value)
{
	return rgbledctrl_setG(device, value);
}

bool rgbledctrl::setB(byte value)
{
	return rgbledctrl_setB(device, value);
}

bool rgbledctrl::eeprom_write(byte data, eepAddr_t address)
{
	return rgbledctrl_eeprom_write(device, data, address);
}

bool rgbledctrl::eeprom_read(byte& data, eepAddr_t address)
{
	return rgbledctrl_eeprom_read(device, &data, address);
}



//
// Getters
//

void rgbledctrl::getVersion(byte (&version)[2])
{
	memcpy(&version, device->version, sizeof(version));
}

eepAddr_t rgbledctrl::getEepromSize()
{
	return device->eepromSize;
}

void rgbledctrl::getColour(s_rgbVal& colour)
{
	memcpy(&colour, &device->rgb, sizeof(s_rgbVal));
}

byte rgbledctrl::getIdleTime()
{
	return device->settings.idleTime;
}

usb_dev_handle* rgbledctrl::getHandle()
{
	return device->handle;
}
