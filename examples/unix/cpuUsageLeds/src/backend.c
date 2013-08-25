/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#define _BSD_SOURCE

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <rgbledctrl.h>
#include "typedefs.h"
#include "backend.h"
#include "util.h"
#include "sine.h"

#define SAMPLERATE			20
#define MIN_RGB_LEVEL		0
#define MAX_RGB_LEVEL		255
#define USB_CHECK_INTERVAL	500
#define GAMMA_CORRECTION	2.2

typedef struct {
	uint count;
	uint idx;
	byte* samples;
}s_sample;

static s_sample sample;
static byte brightness;

static void mainStuff(void);
static byte getCPUUsage(void);
static bool checkDevice(bool);
static void getCPUColour(byte, s_rgbVal*);
static byte getColour(byte, byte, byte, byte, byte);
static void adjustColour(byte*);
static byte getSineVal(byte);

void backend(byte _brightness, ushort transTime)
{
	brightness = _brightness;

	sample.count = (uint)((transTime / SAMPLERATE) + 0.5);
	if(sample.count == 0)
		sample.count = 1;

	sample.samples = realloc(sample.samples, sample.count);
	memset(sample.samples, 0x00, sample.count);

	sample.idx = 0;

	rgbledctrl_init();

	mainStuff();
}

void backend_close()
{
	rgbledctrl_close();
	if(sample.samples)
		free(sample.samples);
}

// Thread that gets and sends CPU usage
static void mainStuff()
{
	bool usbOk = false;
	byte checkCounter = 0;

	while(1)
	{
		usleep(SAMPLERATE * 1000);

		// Get usage
		byte cpuUsage = getCPUUsage();

		// Store sample
		sample.samples[sample.idx++] = cpuUsage;
		if(sample.idx >= sample.count)
			sample.idx = 0;

		// Get average
		uint avg = 0;
		for(uint i=0;i<sample.count;i++)
			avg += sample.samples[i];
		avg /= sample.count;

		cpuUsage = (byte)avg;

		// Show CPU usage
		printf("\n\033[F\033[J");
		printf("Usage: %hhu%%", (byte)((cpuUsage / 255.0) * 100));

		// Constantly searching for USB devices takes up a lot of CPU time,
		// so here we only check for new device every so often.
		if(!usbOk)
		{
			if(++checkCounter < (USB_CHECK_INTERVAL / SAMPLERATE))
				continue;
			checkCounter = 0;
		}

		// Send to USB
		if(checkDevice(usbOk))
		{
			// Workout colour
			s_rgbVal colour;
			getCPUColour(cpuUsage, &colour);

			// Send colour
			usbOk = rgbledctrl_setRGB(&colour);
		}
	}
}

// Get CPU usage and return as a value from 0 to 255
static byte getCPUUsage()
{
	static unsigned long long last_idleTime;
	static unsigned long long last_kernelTime;
	static unsigned long long last_userTime;

	unsigned long long idleTime = 0;
	unsigned long long kernelTime = 0;
	unsigned long long userTime = 0;

	FILE* fp = fopen("/proc/stat","r");
	char line[128];
	if(fp != NULL && fgets(line, 128, fp) != NULL)
	{
		//fputs(line, stdout);
		char* token = strtok(line, " ");
		byte count = 0;
		while(token != NULL && count <= 4)
		{
			//printf("%s\n", token);
			
			switch(count)
			{
				case 1:
					userTime = atoi(token);
					break;
				case 3:
					kernelTime = atoi(token);
					break;
				case 4:
					idleTime = atoi(token);
					break;
			}

			token = strtok(NULL, " ");
			count++;
		}
	}
	else
		perror("File open/read error");
	fclose(fp);

	unsigned long long usr = userTime - last_userTime;
	unsigned long long ker = kernelTime - last_kernelTime;
	unsigned long long idl = idleTime - last_idleTime;

	unsigned long long sys = ker + usr;

	last_idleTime	= idleTime;
	last_kernelTime	= kernelTime;
	last_userTime	= userTime;

	if(sys == 0)
		return 0;

	return (byte)(((float)sys / (sys + idl)) * 255);
}

// See of the device is ok and connected, if not, try reconnecting
static bool checkDevice(bool usbOk)
{
	if(!usbOk || !rgbledctrl_valid())
	{
		if(!rgbledctrl_open())
			return false;
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
	*val = remap(*val, MIN_RGB_LEVEL, MAX_RGB_LEVEL, MIN_RGB_LEVEL, brightness);
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
