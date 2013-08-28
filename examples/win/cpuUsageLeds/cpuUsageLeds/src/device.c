/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#include "win.h"
#include <rgbledctrl.h>
#include "typedefs.h"
#include "device.h"
#include "mcu.h"

//#include <stdio.h>

static void getData(void);
static void brightnessSaveDelay(byte);
static DWORD WINAPI saveDelayThread(LPVOID args);

static HANDLE hWaitTimer;
static HANDLE hSaveDelayThread;
static bool endThread;
static byte saveDelaybrightness;
static s_device device;

// Start threads and everything
void device_init()
{
	rgbledctrl_init();
	device.rgbled_device = NULL;

	endThread			= false;
	hWaitTimer			= CreateWaitableTimer(NULL, FALSE, NULL);
	hSaveDelayThread	= CreateThread(NULL, 0, saveDelayThread, NULL, 0, NULL);
}

// Stop threads etc
void device_end()
{
//	puts("Dev end");

	// device_close() ?

	// Signal thread to end
	endThread = true;

	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -1;
	SetWaitableTimer(hWaitTimer, &liDueTime, 0, NULL, NULL, FALSE);

	WaitForSingleObject(hSaveDelayThread, 3000); // Wait at most 3000ms for thread to end

	CloseHandle(hSaveDelayThread);
	CloseHandle(hWaitTimer);
}

// Open device then get additional data from EEPROM
bool device_open()
{
	rgbledctrl_find();
	device.rgbled_device = rgbledctrl_open();
	if(!device.rgbled_device)
		return false;
	getData();
	return true;
}

void device_close()
{
	rgbledctrl_close(device.rgbled_device);
	device.rgbled_device = NULL;
}

bool device_valid()
{
	return device.rgbled_device != NULL;
}

bool device_poke()
{
	return rgbledctrl_poke(device.rgbled_device);
}

void device_reset()
{
	rgbledctrl_reset(device.rgbled_device);
}

void device_setMode(byte mode)
{
	if(device.ledMode != mode)
	{
		device.ledMode = mode;

		// Disable idle timeout if in single colour mode
		if(mode == MODE_SINGLE_COL)
			device_setIdleTime(IDLETIME_DISABLE);

		// Save mode
		device_eeprom_write(mode, EEPROM_MODE);
	}
}

bool device_setColour(s_rgbVal* colour)
{
	return rgbledctrl_setRGB(device.rgbled_device, colour);
}

void device_setTransitionTime(uint16_t time)
{
	device.transitionTime = time;

	// Save transition time
	device_eeprom_write((byte)(time>>8), EEPROM_TRANSTIME);
	device_eeprom_write((byte)time, EEPROM_TRANSTIME + 1);
}

void device_setBrightness(byte brightness)
{
	device.brightness = brightness;

	// Save brightness with delay
	brightnessSaveDelay(brightness);
}

void device_setIdleTime(byte time)
{
	rgbledctrl_setIdleTime(device.rgbled_device, time);
}

bool device_eeprom_write(byte data, eepAddr_t address)
{
	return rgbledctrl_eeprom_write(device.rgbled_device, data, address);
}

bool device_eeprom_read(byte* data, eepAddr_t address)
{
	return rgbledctrl_eeprom_read(device.rgbled_device, data, address);
}

s_device* device_get()
{
	return &device;
}

// Get EEPROM data
static void getData()
{
	// Read settings from EEPROM
	byte data;

	// Mode
	device_eeprom_read(&data, EEPROM_MODE);
	device.ledMode			= data;

	// Brightness
	device_eeprom_read(&data, EEPROM_BRIGHTNESS);
	device.brightness		= data;

	// Transition time
	device_eeprom_read(&data, EEPROM_TRANSTIME);
	uint16_t time = data << 8;
	device_eeprom_read(&data, EEPROM_TRANSTIME + 1);
	time |= data;
	device.transitionTime	= time;

	// Make sure mode is valid
	if(device.ledMode != MODE_CPU_USAGE && device.ledMode != MODE_SINGLE_COL)
		device_setMode(MODE_CPU_USAGE);

	// Make sure transition time is valid
	if(device.transitionTime == 0)
		device_setTransitionTime(TRANSITIONTIME_DEFAULT);

	saveDelaybrightness		= device.brightness;
}

// Brightness can be set a bunch of times while the track bar slider
// is being moved, this will end up killing the EEPROM.
// This stuff will only save to the EEPROM after 4 seconds of no activity
static void brightnessSaveDelay(byte value)
{
	if(hWaitTimer == NULL)
		return;

	saveDelaybrightness = value;

//	puts("Start save delay");

	// (Re)start timer
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -40000000;
	SetWaitableTimer(hWaitTimer, &liDueTime, 0, NULL, NULL, FALSE);
}

static DWORD WINAPI saveDelayThread(LPVOID args)
{
	UNREFERENCED_PARAMETER(args);

	byte lastBrightness = 0;

	while(WaitForSingleObject(hWaitTimer, INFINITE) == WAIT_OBJECT_0)
	{
		if(saveDelaybrightness != lastBrightness)
		{
			lastBrightness = saveDelaybrightness;
			device_eeprom_write(lastBrightness, EEPROM_BRIGHTNESS);
		}
//		puts("End save delay");
		if(endThread)
			break;
		Sleep(250); // In case something goes crazy
	}

	return EXIT_SUCCESS;
}

