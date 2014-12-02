/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hidapi.h"
#include "rgbled.h"
#include "eeprom.h"
#include "mcu.h"

#define DEBUG_INFO_HID 0

#ifdef _WIN32
	#define LIB_EXPORT __declspec(dllexport)
#else
	#define LIB_EXPORT
#endif

typedef struct rgbled_list_t rgbled_list_t;
struct rgbled_list_t{
	rgbled_list_t* next;	// Next device in list
	char* devPath;			// Device path
	uint32_t id;			// ID
};

// Linked list of RGB LED devices
static rgbled_list_t* rgbLedDevList;

static void clearUsbDevList(void);
static void addUsbDevList(uint32_t, struct hid_device_info*);
static rgbled_t* open(char*);
static void getData(rgbled_t*);
static bool USBget(rgbled_t*, usb_rep_t, void*, size_t);
static bool USBsend(rgbled_t*, usb_rep_t, void*, size_t);
static int doUSBget(hid_device*, usb_rep_t, void*, size_t);
static int doUSBsend(hid_device*, usb_rep_t, void*, size_t);
static bool prepareEEPROM(rgbled_t*, eepAddr_t);

#if DEBUG_INFO_HID
static char* getFeatureReportName(usb_rep_t);
#endif

// Init, must be called before anything else!
void LIB_EXPORT rgbled_init()
{
	clearUsbDevList();

	if(hid_init() < 0)
		puts("HIDAPI Init failed");
}

void LIB_EXPORT rgbled_exit()
{
	clearUsbDevList();
	hid_exit();
}

uint32_t LIB_EXPORT rgbled_find()
{
	uint32_t count = 0;

	clearUsbDevList();

	// Get all HID devices
	struct hid_device_info* allDevices = hid_enumerate(0x0, 0x0);
	struct hid_device_info* currentDevice;
	currentDevice = allDevices;

	// Loop through devices looking for RGB LED Controllers
	while (currentDevice)
	{

#if DEBUG_INFO_HID
		printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", currentDevice->vendor_id, currentDevice->product_id, currentDevice->path, currentDevice->serial_number);
		printf("\n");
		printf("  Manufacturer: %ls\n", currentDevice->manufacturer_string);
		printf("  Product:      %ls\n", currentDevice->product_string);
		printf("  Release:      %hx\n", currentDevice->release_number);
		printf("  Interface:    %d\n",  currentDevice->interface_number);
		printf("\n");
#endif

		if(
			currentDevice->vendor_id == VEN_ID &&
			currentDevice->product_id == DEV_ID &&
			currentDevice->manufacturer_string != NULL &&
			currentDevice->product_string != NULL &&
			wcscmp(currentDevice->manufacturer_string, VEN_NAME) == 0 &&
			wcscmp(currentDevice->product_string, DEV_NAME) == 0
		)
		{
			addUsbDevList(count, currentDevice);
			count++;
		}

		currentDevice = currentDevice->next;
	}

	hid_free_enumeration(allDevices);

	return count;
}

bool LIB_EXPORT rgbled_sameDevice(rgbled_t* dev1, rgbled_t* dev2)
{
	if(!dev1 || !dev2)
		return false;
	else if(!dev1->path || !dev2->path)
		return false;

	return strcmp(dev1->path, dev2->path) == 0;
}

rgbled_t* LIB_EXPORT rgbled_open()
{
	if(rgbLedDevList)
		return open(rgbLedDevList->devPath);
	return NULL;
}

rgbled_t* LIB_EXPORT rgbled_open_byIndex(uint32_t idx)
{
	// Find device with ID
	char* devPath = NULL;
	for(rgbled_list_t* rgbLed = rgbLedDevList; rgbLed; rgbLed = rgbLed->next)
	{
		if(rgbLed->id == idx)
		{
			devPath = rgbLed->devPath;
			break;
		}
	}

	return open(devPath);
}

rgbled_t* LIB_EXPORT rgbled_open_byEEPROM(uint8_t value, eepAddr_t address)
{
	char* devPath = NULL;
	for(rgbled_list_t* rgbLed = rgbLedDevList; rgbLed; rgbLed = rgbLed->next)
	{
		hid_device* handle = hid_open_path(rgbLed->devPath);
		if(!handle)
		{
			fprintf(stderr, "Unable open HID handle. Have you called rgbledctrl_find() recently? (%s)\n", __func__);
			continue;
		}

		rgbled_t device;
		device.handle = handle;
		uint8_t data;
		bool res = rgbled_eeprom_read(&device, &data, address);

		hid_close(handle);

		if(!res)
		{
			fprintf(stderr, "Unable to read EEPROM (%s)\n", __func__);
			continue;
		}

		if(data == value)
		{
			devPath = rgbLed->devPath;
			break;
		}
	}

	return open(devPath);
}

// Close handle
void LIB_EXPORT rgbled_close(rgbled_t* device)
{
	if(!device)
		return;

//	rgbledctrl_eeprom_clear();

	hid_close(device->handle);
	device->handle = NULL;
	free(device);
//	device = NULL;
}

// Send a poke
// Pokes do nothing other than seeing if the device is still connected
bool LIB_EXPORT rgbled_poke(rgbled_t* device)//, bool autoReconnect)
{
	if(!device)
		return false;

	uint8_t data = 0;
	bool valid = false;
	valid = USBget(device, USB_REP_POKE, &data, 1);
	valid = (valid && data == VALUE_POKE);

	return valid;
}

// Reset device (makes the watchdog timer timeout)
bool LIB_EXPORT rgbled_reset(rgbled_t* device)
{
	if(!device)
		return false;
	uint8_t data = VALUE_RESET;
	return USBsend(device, USB_REP_RESET, &data, 1);
}

// Set idle time
bool LIB_EXPORT rgbled_setIdleTime(rgbled_t* device, uint8_t value)
{
	if(!device)
		return false;
	device->settings.idleTime = value;
	return USBsend(device, USB_REP_IDLETIME, &value, 1);
}

// Set red, green and blue values with a single USB request
bool LIB_EXPORT rgbled_setRGB(rgbled_t* device, colour_t* colour)
{
	if(!device)
		return false;

	memcpy(&device->colour, colour, sizeof(colour_t));

	uint8_t data[3];
	data[0] = colour->r;
	data[1] = colour->g;
	data[2] = colour->b;

	return USBsend(device, USB_REP_LED_RGB, data, sizeof(data));
}

// Set red value
bool LIB_EXPORT rgbled_setR(rgbled_t* device, uint8_t value)
{
	if(!device)
		return false;
	device->colour.r = value;
	return USBsend(device, USB_REP_LED_R, &value, sizeof(value));
}

// Set green value
bool LIB_EXPORT rgbled_setG(rgbled_t* device, uint8_t value)
{
	if(!device)
		return false;
	device->colour.g = value;
	return USBsend(device, USB_REP_LED_G, &value, sizeof(value));
}

// Set blue value
bool LIB_EXPORT rgbled_setB(rgbled_t* device, uint8_t value)
{
	if(!device)
		return false;
	device->colour.b = value;
	return USBsend(device, USB_REP_LED_B, &value, sizeof(value));
}

// Clear linked list of all devices
static void clearUsbDevList()
{
	rgbled_list_t* rgbLedDev = rgbLedDevList;
	while(rgbLedDev)
	{
		free(rgbLedDev->devPath);

		rgbled_list_t* temp = rgbLedDev;
		rgbLedDev = rgbLedDev->next;
		free(temp);
	}
	rgbLedDevList = NULL;
}

// Add a device to linked list
static void addUsbDevList(uint32_t id, struct hid_device_info* dev)
{
	rgbled_list_t* rgbLedDev = rgbLedDevList;
	if(rgbLedDev != NULL) // Root is defined
	{
		// Find last item
		for(;rgbLedDev->next;rgbLedDev = rgbLedDev->next);

		// Make next item
		rgbLedDev->next = malloc(sizeof(rgbled_list_t));
		rgbLedDev = rgbLedDev->next;
	}
	else // No root, need to make it
	{
		// Make root item
		rgbLedDevList = malloc(sizeof(rgbled_list_t));
		rgbLedDev = rgbLedDevList;
	}

	// Assign data
	rgbLedDev->next = NULL;
	rgbLedDev->devPath = malloc(strlen(dev->path) + 1);
	strcpy(rgbLedDev->devPath, dev->path);
	rgbLedDev->id = id;
}

// Open handle to RGB LED controller
static rgbled_t* open(char* devPath)
{
	if(!devPath)
		return NULL;

	// Open device
	hid_device* handle = hid_open_path(devPath);
	if(!handle)
		return NULL;

	// Store device info
	rgbled_t* device = calloc(1, sizeof(rgbled_t));
	device->handle = handle;
	device->path = malloc(strlen(devPath) + 1);
	strcpy(device->path, devPath);

	// Get additional info
	getData(device);

	return device;
}

// Get device info, version, EEPROM size etc
static void getData(rgbled_t* device)
{
	uint8_t buffer[4];

	// Current colour
	memset(buffer, 0x00, sizeof(buffer));
	if(USBget(device, USB_REP_LED_RGB, buffer, 3))
		memcpy(&device->colour, buffer, sizeof(colour_t));

	// Idle time
	memset(buffer, 0x00, sizeof(buffer));
	if(USBget(device, USB_REP_IDLETIME, buffer, 1))
		device->settings.idleTime	= buffer[0];

	// Info (version, EEPROM size)
	memset(buffer, 0x00, sizeof(buffer));
	if(USBget(device, USB_REP_DEVINFO, buffer, 4))
	{
		device->version.major		= buffer[1];
		device->version.minor		= buffer[0];
		device->eepromSize			= (eepAddr_t)(buffer[2] | (buffer[3]<<8));
	}
}

static bool USBget(rgbled_t* device, usb_rep_t reportId, void* data, size_t len)
{
	if(!device)
		return false;
	return doUSBget(device->handle, reportId, data, len) >= 0;
}


static bool USBsend(rgbled_t* device, usb_rep_t reportId, void* data, size_t len)
{
	if(!device)
		return false;
	return doUSBsend(device->handle, reportId, data, len) >= 0;
}

static int doUSBget(hid_device* handle, usb_rep_t reportId, void* data, size_t len)
{
	if(!handle)
		return -1;

	size_t reportLen = len + 1; // Add 1 for report ID

	uint8_t reportData[reportLen];
	memset(reportData, 0x00, reportLen); // Clear report buffer

	reportData[0] = reportId; // Set the report ID

	// Docs say that the returned data includes the report ID at [0], but that doesn't seem to be the case, however res is the returned data size + 1 :/
	// Unless no data is returned by the device, then res will be 1 and [0] will be the report ID.

	// Get the report
	int res = hid_get_feature_report(handle, reportData, reportLen);

#if DEBUG_INFO_HID
	printf("---- Get Feature Report ----\n");
	printf("  ID: %hhu (%s)\n", reportId, getFeatureReportName(reportId));
	printf("  Len: %d\n", res);
	printf("  ");
	for (uint8_t i = 0; i < res; i++)
		printf("%02hhx ", reportData[i]);
	printf("\n");
#endif

// Linux returns len, win returns reportLen

	// If returned data is the correct size then copy to data buffer, eh, mixing signed and unsigned here :/
#ifdef _WIN32
	if((size_t)res == reportLen)
#else
	if((size_t)res == len)
#endif
		memcpy(data, reportData, res);
	else if(res < 0) // Show any errors
		printf("ERR (get): %ls\n", hid_error(handle));
	else
		printf("ERR (get): Data error, returned report length (%d) does not match requested length (%u)\n", res, reportLen);

	return res;
}

static int doUSBsend(hid_device* handle, usb_rep_t reportId, void* data, size_t len)
{
	if(!handle)
		return -1;
	else if(data == NULL) // We need something to send
		return -1;

	size_t reportLen = len + 1; // Add 1 for report ID

	uint8_t reportData[reportLen];
	memcpy(reportData + 1, data, len); // Copy data for sending to report buffer

	reportData[0] = reportId; // Set the report ID

	// Send the report
	int res = hid_send_feature_report(handle, reportData, reportLen);

#if DEBUG_INFO_HID
	// Debug info
	printf("---- Send Feature Report ----\n");
	printf("  ID: %hhu (%s)\n", reportId, getFeatureReportName(reportId));
	printf("  Len: %d\n", res);
	printf("  ");
	for (uint8_t i = 0; i < res; i++)
		printf("%02hhx ", reportData[i]);
	printf("\n");
#endif

	// Show any errors
	if(res < 0)
		printf("ERR (send): %ls\n", hid_error(handle));

	return res;
}

/*
void rgbledctrl_eeprom_init(uint commitTime)
{

}
*/

// Write a byte to EEPROM
bool LIB_EXPORT rgbled_eeprom_write(rgbled_t* device, uint8_t data, eepAddr_t address)
{
	// Set address to write to
	if(!prepareEEPROM(device, address))
		return false;

	// Send the data
	return USBsend(device, USB_REP_EEP, &data, 1);
}

// Read a byte from EEPROM
bool LIB_EXPORT rgbled_eeprom_read(rgbled_t* device, uint8_t* data, eepAddr_t address)
{
	// Set address to read from
	if(!prepareEEPROM(device, address))
		return false;

	// Read the data
	return USBget(device, USB_REP_EEP, data, 1);
}

static bool prepareEEPROM(rgbled_t* device, eepAddr_t address)
{
	if(!device)
		return false;
	else if(address >= device->eepromSize) // Make sure EEPROM address is valid
	{
		fprintf(stderr, "EEPROM Address out of range (Address=%hu EEPROM=%hu)\n", address, device->eepromSize);
		return false;
	}

	uint8_t addr[2];
	addr[0] = address;
	addr[1] = address>>8;

	// Send address
	return USBsend(device, USB_REP_EEP_SETUP, addr, 2);
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

#if DEBUG_INFO_HID
static char* getFeatureReportName(usb_rep_t id)
{
	static char featureName[20];

	switch(id)
	{
	case USB_REP_POKE:
		strcpy(featureName, "Poke");
		break;
	case USB_REP_DEVINFO:
		strcpy(featureName, "Device info");
		break;
	case USB_REP_RESET:
		strcpy(featureName, "Reset");
		break;
	case USB_REP_LED_RGB:
		strcpy(featureName, "LED Colour");
		break;
	case USB_REP_LED_R:
		strcpy(featureName, "LED R");
		break;
	case USB_REP_LED_G:
		strcpy(featureName, "LED G");
		break;
	case USB_REP_LED_B:
		strcpy(featureName, "LED B");
		break;
	case USB_REP_IDLETIME:
		strcpy(featureName, "Idle time");
		break;
	case USB_REP_EEP_SETUP:
		strcpy(featureName, "Setup EEPROM");
		break;
	case USB_REP_EEP:
		strcpy(featureName, "Read/write EEPROM");
		break;
	default:
		strcpy(featureName, "Unknown");
		break;
	}

	return featureName;
}
#endif
