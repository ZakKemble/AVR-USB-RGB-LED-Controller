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
#include <stdio.h>
#include <string.h>
#include "rgbledctrl.h"
#include "eeprom.h"
#include "mcu.h"
#include "opendevice.h"

#define USB_TIMEOUT 2000

typedef unsigned int uint;
typedef unsigned short ushort;

static s_rgbled_device device;

static void getData(void);
static bool USBsend(byte, int, int, char*, uint);
static int doUSBsend(byte, int, int, char*, uint);

// Init, must be called before anything else!
void rgbledctrl_init()
{
	usb_init();
}

// Open handle to RGB LED controller
bool rgbledctrl_open()
{
	rgbledctrl_close();
	bool valid = (usbOpenDevice(&device.handle, VEN_ID, VEN_NAME, DEV_ID, DEV_NAME, "*", NULL, stderr) == USBOPEN_SUCCESS);
	if(valid)
		getData();
	return valid;
}

// Close handle
void rgbledctrl_close()
{
//	rgbledctrl_eeprom_clear();
	if(rgbledctrl_valid())
	{
		usb_close(device.handle);
		device.handle = NULL;
	}
}

// Check handle
bool rgbledctrl_valid()
{
	return (device.handle != NULL);
}

// Send a poke
// Pokes do nothing other than seeing if the device is still connected
// This function can also try reconnecting if poke failed
bool rgbledctrl_poke(bool autoReconnect)
{
	bool valid = false;

	if(rgbledctrl_valid())
		valid = USBsend(USB_REQ_POKE, 0, 0, NULL, 0);

	if(!valid && autoReconnect)
		valid = rgbledctrl_open();

	return valid;
}

// Reset device (makes the watchdog timer timeout)
bool rgbledctrl_reset()
{
	return USBsend(USB_REQ_RESET, 0, 0, NULL, 0);
}

// Set idle time
bool rgbledctrl_setIdleTime(byte value)
{
	device.settings.idleTime = value;
	return USBsend(USB_REQ_IDLETIME, value, 0, NULL, 0);
}

// Set red, green and blue values with a single USB request
bool rgbledctrl_setRGB(s_rgbVal* colour)
{
	memcpy(&device.rgb, colour, sizeof(s_rgbVal));
	int data1 = (colour->green<<8) | colour->red;
	return USBsend(USB_REQ_LED_RGB, data1, colour->blue, NULL, 0);
}

// Set red value
bool rgbledctrl_setR(byte value)
{
	device.rgb.red = value;
	return USBsend(USB_REQ_LED_R, value, 0, NULL, 0);
}

// Set green value
bool rgbledctrl_setG(byte value)
{
	device.rgb.green = value;
	return USBsend(USB_REQ_LED_G, value, 0, NULL, 0);
}

// Set blue value
bool rgbledctrl_setB(byte value)
{
	device.rgb.blue = value;
	return USBsend(USB_REQ_LED_B, value, 0, NULL, 0);
}

// Get pointer to device stuff
s_rgbled_device* rgbledctrl_getDevice()
{
	return &device;
}

// Get device info, version, eeprom size etc
static void getData()
{
	byte buffer[8];
	if(USBsend(USB_REQ_DEVSTATE, 0, 0, (char*)buffer, sizeof(buffer)))
	{
		device.version[0]			= buffer[1];
		device.version[1]			= buffer[0];
		device.eepromSize			= (eepAddr_t)(buffer[2]<<8 | buffer[3]);
		device.settings.idleTime	= buffer[4];
		device.rgb.red				= buffer[5];
		device.rgb.green			= buffer[6];
		device.rgb.blue				= buffer[7];
	}
}

static bool USBsend(byte reqType, int data1, int data2, char* out, uint size)
{
	return doUSBsend(reqType, data1, data2, out, size) >= 0;
}

static int doUSBsend(byte reqType, int data1, int data2, char* out, uint size)
{
	if(!rgbledctrl_valid())
		return -1;

	return usb_control_msg(device.handle,
		USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
		reqType, data1, data2, out, size, USB_TIMEOUT);
}


/*
void rgbledctrl_eeprom_init(uint commitTime)
{

}
*/

// Write a byte to EEPROM
bool rgbledctrl_eeprom_write(byte value, eepAddr_t address)
{
//	eeprom_write(value, address);
	return USBsend(USB_REQ_EEP_WRITE, value, address, NULL, 0);
}

// Read a byte from EEPROM
bool rgbledctrl_eeprom_read(byte* data, eepAddr_t address)
{
//	return eeprom_read(address);
	return USBsend(USB_REQ_EEP_READ, 0, address, (char*)data, sizeof(data));
}
/*
// Clear all EEPROM data (doesn't send to MCU)
void rgbledctrl_eeprom_clear()
{
	eeprom_clear();
}

// Flush all EEPROM data to MCU
void rgbledctrl_eeprom_commit()
{
	eeprom_commit();
}
*/

