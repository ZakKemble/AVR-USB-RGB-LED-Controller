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

#define USB_TIMEOUT 2000

typedef struct usb_device usb_dev;
typedef struct s_rgbled_list s_rgbled_list;
struct s_rgbled_list{
	s_rgbled_list* next;
	usb_dev* dev;
	uint id;
};

// Linked list of RGB LED devices
static s_rgbled_list* rgbLedDevList;

static void clearUsbDevList(void);
static void addUsbDevList(int, usb_dev*);
static int usbGetStringAscii(usb_dev_handle*, int , char*, int);
static s_rgbled_device* open(usb_dev*);
static void getData(s_rgbled_device*);
static bool USBsend(s_rgbled_device*, byte, int, int, char*, uint);
static int doUSBsend(usb_dev_handle*, byte, int, int, char*, uint);

// Init, must be called before anything else!
void rgbledctrl_init()
{
	rgbLedDevList = NULL;
	usb_init();
}

// Based on V-USB example code by Christian Starkjohann
uint rgbledctrl_find()
{
	uint count = 0;

	clearUsbDevList();

	usb_find_busses();
	usb_find_devices();

	for(struct usb_bus* bus = usb_get_busses(); bus; bus = bus->next)
	{
		for(usb_dev* dev = bus->devices; dev; dev = dev->next)
		{
			// Check vendor and product IDs
			if(dev->descriptor.idVendor != VEN_ID || dev->descriptor.idProduct != DEV_ID)
				continue;

			// Open device
			usb_dev_handle* handle = usb_open(dev);
			if(!handle)
			{
				fprintf(stderr, "Warning: cannot open VID=0x%04x PID=0x%04x: %s\n", dev->descriptor.idVendor, dev->descriptor.idProduct, usb_strerror());
				continue;
			}

			int len;

			// Check vendor/manufacturer ID
			char vendor[256];
			len = 0;
			vendor[0] = 0x00;
			if(dev->descriptor.iManufacturer > 0)
				len = usbGetStringAscii(handle, dev->descriptor.iManufacturer, vendor, sizeof(vendor));

			if(len >= 0)
			{
				if(strcmp(vendor, VEN_NAME) == 0)
				{
					// Check product/device ID
					char product[256];
					len = 0;
					product[0] = 0x00;
					if(dev->descriptor.iProduct > 0)
						len = usbGetStringAscii(handle, dev->descriptor.iProduct, product, sizeof(product));

					if(len >= 0)
					{
						if(strcmp(product, DEV_NAME) == 0)
						{
							addUsbDevList(count, dev);
							count++;
						}
					}
					else
						fprintf(stderr, "Warning: cannot query product for VID=0x%04x PID=0x%04x: %s\n", dev->descriptor.idVendor, dev->descriptor.idProduct, usb_strerror());
				}
			}
			else
				fprintf(stderr, "Warning: cannot query manufacturer for VID=0x%04x PID=0x%04x: %s\n", dev->descriptor.idVendor, dev->descriptor.idProduct, usb_strerror());

			usb_close(handle);
		}
	}

	return count;
}

bool rgbledctrl_sameDevice(usb_dev_handle* handle1, usb_dev_handle* handle2)
{
	if(!handle1 || !handle2)
		return false;

	usb_dev* dev1 = usb_device(handle1);
	usb_dev* dev2 = usb_device(handle2);
	return dev1 == dev2;
}

s_rgbled_device* rgbledctrl_open()
{
	if(rgbLedDevList)
		return open(rgbLedDevList->dev);
	return NULL;
}

s_rgbled_device* rgbledctrl_open_byIndex(uint idx)
{
	// Find device with ID
	usb_dev* dev = NULL;
	for(s_rgbled_list* rgbLed = rgbLedDevList; rgbLed; rgbLed = rgbLed->next)
	{
		if(rgbLed->id == idx)
		{
			dev = rgbLed->dev;
			break;
		}
	}

	return open(dev);
}

s_rgbled_device* rgbledctrl_open_byEEPROM(byte value, eepAddr_t address)
{
	usb_dev* dev = NULL;
	for(s_rgbled_list* rgbLed = rgbLedDevList; rgbLed; rgbLed = rgbLed->next)
	{
		usb_dev_handle* handle = usb_open(rgbLed->dev);
		if(!handle)
		{
			fprintf(stderr, "Unable open USB handle. Have you called rgbledctrl_find() recently? (%s)\n", __func__);
			continue;
		}

		s_rgbled_device device;
		device.handle = handle;
		byte data;
		bool res = rgbledctrl_eeprom_read(&device, &data, address);

		usb_close(handle);

		if(!res)
		{
			fprintf(stderr, "Unable to read EEPROM (%s)\n", __func__);
			continue;
		}

		if(data == value)
		{
			dev = rgbLed->dev;
			break;
		}
	}

	return open(dev);
}

// Close handle
void rgbledctrl_close(s_rgbled_device* device)
{
	if(!device)
		return;

//	rgbledctrl_eeprom_clear();

	usb_close(device->handle);
	device->handle = NULL;
	free(device);
//	device = NULL;
}

// Send a poke
// Pokes do nothing other than seeing if the device is still connected
bool rgbledctrl_poke(s_rgbled_device* device)//, bool autoReconnect)
{
	if(!device)
		return false;

	bool valid = false;

	valid = USBsend(device, USB_REQ_POKE, 0, 0, NULL, 0);
/*
	if(!valid && autoReconnect)
	{
		s_rgbled_device* newDevice = open(device);
		if(newDevice)
		{

		}
//		valid = rgbledctrl_open(device);
	}
*/
	return valid;
}

// Reset device (makes the watchdog timer timeout)
bool rgbledctrl_reset(s_rgbled_device* device)
{
	if(!device)
		return false;
	return USBsend(device, USB_REQ_RESET, 0, 0, NULL, 0);
}

// Set idle time
bool rgbledctrl_setIdleTime(s_rgbled_device* device, byte value)
{
	if(!device)
		return false;
	device->settings.idleTime = value;
	return USBsend(device, USB_REQ_IDLETIME, value, 0, NULL, 0);
}

// Set red, green and blue values with a single USB request
bool rgbledctrl_setRGB(s_rgbled_device* device, s_rgbVal* colour)
{
	if(!device)
		return false;
	memcpy(&device->rgb, colour, sizeof(s_rgbVal));
	int data1 = (colour->green<<8) | colour->red;
	return USBsend(device, USB_REQ_LED_RGB, data1, colour->blue, NULL, 0);
}

// Set red value
bool rgbledctrl_setR(s_rgbled_device* device, byte value)
{
	if(!device)
		return false;
	device->rgb.red = value;
	return USBsend(device, USB_REQ_LED_R, value, 0, NULL, 0);
}

// Set green value
bool rgbledctrl_setG(s_rgbled_device* device, byte value)
{
	if(!device)
		return false;
	device->rgb.green = value;
	return USBsend(device, USB_REQ_LED_G, value, 0, NULL, 0);
}

// Set blue value
bool rgbledctrl_setB(s_rgbled_device* device, byte value)
{
	if(!device)
		return false;
	device->rgb.blue = value;
	return USBsend(device, USB_REQ_LED_B, value, 0, NULL, 0);
}

// Clear linked list of all devices
static void clearUsbDevList()
{
	s_rgbled_list* rgbLedDev = rgbLedDevList;
	while(rgbLedDev)
	{
		s_rgbled_list* temp = rgbLedDev;
		rgbLedDev = rgbLedDev->next;
		free(temp);
	}
	rgbLedDevList = NULL;
}

// Add a device to linked list
static void addUsbDevList(int id, usb_dev* dev)
{
	s_rgbled_list* rgbLedDev = rgbLedDevList;
	if(rgbLedDev)
	{
		// Find last item
		for(;rgbLedDev->next;rgbLedDev = rgbLedDev->next){};

		// Make next item
		rgbLedDev->next = malloc(sizeof(s_rgbled_list));
		rgbLedDev = rgbLedDev->next;
	}
	else
	{
		// Make root item
		rgbLedDevList = malloc(sizeof(s_rgbled_list));
		rgbLedDev = rgbLedDevList;
	}

	// Assign data
	rgbLedDev->next = NULL;
	rgbLedDev->dev = dev;
	rgbLedDev->id = id;
}

// Based on V-USB example code by Christian Starkjohann
static int usbGetStringAscii(usb_dev_handle* dev, int index, char *buf, int buflen)
{
	char buffer[256];
	int rval, i;

	if((rval = usb_get_string_simple(dev, index, buf, buflen)) >= 0) /* use libusb version if it works */
		return rval;
	else if((rval = usb_control_msg(dev, USB_ENDPOINT_IN, USB_REQ_GET_DESCRIPTOR, (USB_DT_STRING << 8) + index, 0x0409, buffer, sizeof(buffer), USB_TIMEOUT)) < 0)
		return rval;

	if(buffer[1] != USB_DT_STRING)
	{
		*buf = 0;
		return 0;
	}

	if((unsigned char)buffer[0] < rval)
		rval = (unsigned char)buffer[0];

	rval /= 2;

	/* lossy conversion to ISO Latin1: */
	for(i=1;i<rval && i < buflen;i++)
	{
		if(buffer[2 * i + 1] == 0)
			buf[i - 1] = buffer[2 * i];
		else  /* outside of ISO Latin1 range */
			buf[i - 1] = '?';
	}
	buf[i - 1] = 0;

	return i - 1;
}

// Open handle to RGB LED controller
static s_rgbled_device* open(usb_dev* dev)
{
	if(!dev)
		return NULL;

	usb_dev_handle* handle = usb_open(dev);
	if(!handle)
		return NULL;

	s_rgbled_device* device = calloc(1, sizeof(s_rgbled_device));
	device->handle = handle;
	getData(device);
	return device;
}

// Get device info, version, EEPROM size etc
static void getData(s_rgbled_device* device)
{
	byte buffer[8];
	if(USBsend(device, USB_REQ_DEVSTATE, 0, 0, (char*)buffer, sizeof(buffer)))
	{
		device->version[0]			= buffer[1];
		device->version[1]			= buffer[0];
		device->eepromSize			= (eepAddr_t)(buffer[2]<<8 | buffer[3]);
		device->settings.idleTime	= buffer[4];
		memcpy(&device->rgb, &buffer[5], sizeof(s_rgbVal));
	}
}

static bool USBsend(s_rgbled_device* device, byte reqType, int data1, int data2, char* out, uint size)
{
	if(!device)
		return false;
	return doUSBsend(device->handle, reqType, data1, data2, out, size) >= 0;
}

static int doUSBsend(usb_dev_handle* handle, byte reqType, int data1, int data2, char* out, uint size)
{
	if(!handle)
		return -1;

	return usb_control_msg(handle,
		USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
		reqType, data1, data2, out, size, USB_TIMEOUT);
}


/*
void rgbledctrl_eeprom_init(uint commitTime)
{

}
*/

// Write a byte to EEPROM
bool rgbledctrl_eeprom_write(s_rgbled_device* device, byte data, eepAddr_t address)
{
	if(!device)
		return false;
	else if(address >= device->eepromSize)
	{
		fprintf(stderr, "EEPROM Write address out of range (Address=%hu EEPROM=%hu)\n", address, device->eepromSize);
		return false;
	}

//	eeprom_write(value, address);
	return USBsend(device, USB_REQ_EEP_WRITE, data, address, NULL, 0);
}

// Read a byte from EEPROM
bool rgbledctrl_eeprom_read(s_rgbled_device* device, byte* data, eepAddr_t address)
{
	if(!device)
		return false;
	else if(address >= device->eepromSize)
	{
		fprintf(stderr, "EEPROM Read address out of range (Address=%hu EEPROM=%hu)\n", address, device->eepromSize);
		return false;
	}

//	return eeprom_read(address);
	return USBsend(device, USB_REQ_EEP_READ, 0, address, (char*)data, sizeof(data));
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

