/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#include "win.h"
#include <stdlib.h>
#include <math.h>
#include "typedefs.h"
#include "backend.h"
#include "device.h"
#include "actions.h"
#include "resource.h"
#include "mcu.h"
#include "util.h"
#include "sine.h"

#define SAMPLERATE			20 // Minimum sample rate is 16
#define MIN_RGB_LEVEL		0
#define MAX_RGB_LEVEL		255
#define USB_CHECK_INTERVAL	500
#define USB_POKE_INTERVAL	500
#define GAMMA_CORRECTION	2.2

typedef struct {
	uint count;
	uint idx;
	byte* samples;
}s_sample;

static HANDLE hThreadExitEvent;
static HANDLE hMainThread;
static CRITICAL_SECTION cs;
static s_sample sample;

static DWORD WINAPI mainThread(LPVOID);
static byte getCPUUsage(void);
static bool checkDevice(bool);
static void getCPUColour(byte, s_rgbVal*);
static byte getColour(byte, byte, byte, byte, byte);
static void adjustColour(byte*);
static byte getSineVal(byte);

void backend()
{
	InitializeCriticalSection(&cs);

	backend_setTransitionTime(TRANSITIONTIME_DEFAULT);

	hThreadExitEvent	= CreateEvent(NULL, TRUE, FALSE, NULL);
	hMainThread			= CreateThread(NULL, 0, mainThread, NULL, 0, NULL);

	device_init();
}

void backend_close()
{
	// BUG: If event is set when its not being waited on, WaitForSingleObject()
	// will get stuck/timeout as if the event was never set.

	SetEvent(hThreadExitEvent); // Signal thread to end
	WaitForSingleObject(hMainThread, 3000); // Wait at most 3000ms for thread to end

	CloseHandle(hMainThread);
	CloseHandle(hThreadExitEvent);
	DeleteCriticalSection(&cs);

	device_end();
	device_close();
}
/*
void backend_wait()
{
	WaitForSingleObject(hMainThread, INFINITE);
}
*/
void backend_deviceReset()
{
	device_reset();
}

void backend_setMode(byte mode)
{
	device_setMode(mode);
}

void backend_setColour(byte r, byte g, byte b)
{
	// Gamma correction
	r = (byte)(pow(r / 255.0, GAMMA_CORRECTION) * 255);
	g = (byte)(pow(g / 255.0, GAMMA_CORRECTION) * 255);
	b = (byte)(pow(b / 255.0, GAMMA_CORRECTION) * 255);

	s_rgbVal colour = {r, g, b};
	device_setColour(&colour);
}

void backend_setTransitionTime(uint time)
{
	device_setTransitionTime(time);

	// Realloc samples and stuff
	EnterCriticalSection(&cs);

	sample.count = (uint)((time / SAMPLERATE) + 0.5);
	if(sample.count == 0)
		sample.count = 1;

	sample.samples = realloc(sample.samples, sample.count);
	memset(sample.samples, 0x00, sample.count);

	sample.idx = 0;

	LeaveCriticalSection(&cs);
}

void backend_setBrightness(byte brightness)
{
	device_setBrightness(brightness);
}

void backend_setIdleTime(byte time)
{
	device_setIdleTime(time);
}

// Thread that gets and sends CPU usage
static DWORD WINAPI mainThread(LPVOID args)
{
	UNREFERENCED_PARAMETER(args);

	bool usbOk = true;
	byte checkCounter = 0;
	byte pokeCounter = 0;

	while(1)
	{
		// If exit event is set then exit thread, otherwise wait for timeout and do the usual CPU stuff
		if(WaitForSingleObject(hThreadExitEvent, SAMPLERATE) != WAIT_TIMEOUT)
			break;

		// Get usage
		byte cpuUsage = getCPUUsage();

		EnterCriticalSection(&cs);

		// Store sample
		sample.samples[sample.idx++] = cpuUsage;
		if(sample.idx >= sample.count)
			sample.idx = 0;

		// Get average
		uint avg = 0;
		for(uint i=0;i<sample.count;i++)
			avg += sample.samples[i];
		avg /= sample.count;

		LeaveCriticalSection(&cs);

		cpuUsage = (byte)avg;

		// Show CPU usage
		actions_showCPULoad(cpuUsage);

		// Constantly searching for USB devices takes up a lot of CPU time,
		// so here we only check for new device every so often.
		if(!usbOk)
		{
			if(++checkCounter < (USB_CHECK_INTERVAL / SAMPLERATE))
				continue;
			checkCounter = 0;
		}

		// Send to USB
		if((usbOk = checkDevice(usbOk)))
		{
			if(device_get()->ledMode == MODE_CPU_USAGE)
			{
				// Workout colour
				s_rgbVal colour;
				getCPUColour(cpuUsage, &colour);

				// Send colour
				usbOk = device_setColour(&colour);
			}
			else
			{
				// Just send pokes if not in CPU usage mode,
				// this sees if the device is still connected
				if(++pokeCounter >= (USB_POKE_INTERVAL / SAMPLERATE))
				{
					pokeCounter = 0;
					usbOk = device_poke();
				}
			}
		}
	}

	return EXIT_SUCCESS;
}

// Get CPU usage and return as a value from 0 to 255
static byte getCPUUsage()
{
	static unsigned long long last_idleTime;
	static unsigned long long last_kernelTime;
	static unsigned long long last_userTime;

	unsigned long long idleTime;
	unsigned long long kernelTime;
	unsigned long long userTime;
	bool res = GetSystemTimes((LPFILETIME)&idleTime, (LPFILETIME)&kernelTime, (LPFILETIME)&userTime);
	UNREFERENCED_PARAMETER(res);

	unsigned long long idl = idleTime - last_idleTime;
	unsigned long long ker = kernelTime - last_kernelTime;
	unsigned long long usr = userTime - last_userTime;

	unsigned long long sys = ker + usr;

	last_idleTime	= idleTime;
	last_kernelTime	= kernelTime;
	last_userTime	= userTime;

	if(sys == 0)
		return 0;

	return (byte)((sys - idl) * 255 / sys);
}

// See of the device is ok and connected, if not, try reconnecting
static bool checkDevice(bool usbOk)
{
	if(!usbOk || !device_valid())
	{
		device_close();
		device_open();
		actions_showConnectionStatus();
		if(!device_valid())
			return false;
		actions_showCurrentSettings();
		actions_setTransitionTime();
	}
	return true;
}

static void getCPUColour(byte cpuUsage, s_rgbVal* colour)
{
	// Workout colour
	colour->red		= getColour(cpuUsage, 128, 255, MIN_RGB_LEVEL, MAX_RGB_LEVEL);
	colour->blue	= getColour(cpuUsage, 0, 127, MAX_RGB_LEVEL, MIN_RGB_LEVEL);

	if(cpuUsage < 128)
		colour->green = getColour(cpuUsage, 0, 127, MIN_RGB_LEVEL, MAX_RGB_LEVEL);
	else
		colour->green = getColour(cpuUsage, 128, 255, MAX_RGB_LEVEL, MIN_RGB_LEVEL);

	// Adjust colour (sine smooth, brightness)
	adjustColour(&colour->red);
	adjustColour(&colour->blue);
	adjustColour(&colour->green);
}

static byte getColour(byte cpuUsage, byte inMin, byte inMax, byte outMin, byte outMax)
{
	return remap(
		limitVal(
			cpuUsage,
			inMin,
			inMax),
		inMin,
		inMax,
		outMin,
		outMax
	);
}

static void adjustColour(byte* val)
{
	// Adjust to sine level
	*val = getSineVal(*val);

	// Gamma correction
    *val = (byte)(pow(*val / 255.0, GAMMA_CORRECTION) * 255);

	// Adjust for brightness
	*val = remap(*val, MIN_RGB_LEVEL, MAX_RGB_LEVEL, MIN_RGB_LEVEL, device_get()->brightness);
}

// Makes a smoother transition between colours
static byte getSineVal(byte idx)
{
	byte origIdx = idx;

	idx -= 128;
	if(idx > 127)
		idx = 255 - idx;

	byte val = sineVals[idx];

	if(origIdx > 127)
		val += 127;
	else
		val = abs(val - 127);
	return val;
}
