/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#ifdef _WIN32
#include "win.h"
#include <Shlwapi.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <rgbledctrl.h>

static void incArgIdx(int*, int);
static int getArgVal(int*, int, char**, int);

int main(int argc, char **argv)
{
	s_rgbVal colour;
	bool getInfo					= false;
	bool setColour					= false;
	bool setIdleTime				= false;
	bool readEEPROM					= false;
	bool writeEEPROM				= false;
	byte idleTime;
	eepAddr_t readEEPROMAddress;
	eepAddr_t writeEEPROMAddress;
	byte writeEEPROMData;

	// Check args
	for(int argIdx = 1;argIdx<argc;argIdx++)
	{
		if(strcmp(argv[argIdx], "-h") == 0 || strcmp(argv[argIdx], "--help") == 0)
		{
#ifdef _WIN32
			TCHAR* programName;
			TCHAR szFileName[MAX_PATH];
			GetModuleFileName(NULL, szFileName, MAX_PATH);
			programName = PathFindFileName(szFileName);
#else
			char* programName = argv[0];
#endif

			printf("Usage: %s [--help|-h] [getinfo] [colour <red> <green> <blue>] [idletime <time>] [eeprom r <address>] [eeprom w <address> <value>]\n", programName);
			puts("");
			puts("Examples:");
			puts("");
			puts("Set colour purple:");
			printf("%s colour 255 0 255\n", programName);
			puts("");
			puts("Write 43 to EEPROM address 5:");
			printf("%s eeprom w 5 43\n", programName);
			puts("");
			puts("Read EEPROM address 5:");
			printf("%s eeprom r 5\n", programName);
			exit(EXIT_SUCCESS);
		}
		else if(strcmp(argv[argIdx], "getinfo") == 0)
			getInfo = true;
		else if(strcmp(argv[argIdx], "colour") == 0)
		{
			setColour = true;
			colour.red = getArgVal(&argIdx, argc, argv, 255);
			colour.green = getArgVal(&argIdx, argc, argv, 255);
			colour.blue = getArgVal(&argIdx, argc, argv, 255);
		}
		else if(strcmp(argv[argIdx], "idletime") == 0)
		{
			setIdleTime = true;
			idleTime = getArgVal(&argIdx, argc, argv, 255);
		}
		else if(strcmp(argv[argIdx], "eeprom") == 0)
		{
			incArgIdx(&argIdx, argc);
			if(strcmp(argv[argIdx], "r") == 0)
			{
				readEEPROM = true;
				readEEPROMAddress = getArgVal(&argIdx, argc, argv, 0); // Address
			}
			else if(strcmp(argv[argIdx], "w") == 0)
			{
				writeEEPROM = true;
				writeEEPROMAddress = getArgVal(&argIdx, argc, argv, 0); // Address
				writeEEPROMData = getArgVal(&argIdx, argc, argv, 255); // Data
			}
			else
			{
				fprintf(stderr, "Unknown EEPROM operation: %s\n", argv[argIdx]);
				exit(EXIT_FAILURE);
			}
		}
	}

	// Initialise
	rgbledctrl_init();

	// Find controllers
	uint count = rgbledctrl_find();
	printf("RGB LED Controllers found: %d\n", count);

	// Loop over found controllers
	for(uint i=0;i<count;i++)
	{
		printf("Opening device (Index: %u)\n", i);
		s_rgbled_device* rgbLed = rgbledctrl_open_byIndex(i); // Try opening

		if(!rgbLed) // Failed to open
		{
			fprintf(stderr, "Could not open device (Index: %u)\n", i);
			continue;
		}

		printf("Device opened (Index: %u)\n", i);

		if(getInfo)
		{
			// Version
			printf("Firmware version: %hhu.%hhu\n", rgbLed->version[0], rgbLed->version[1]);

			// User EEPROM size
			printf("User EEPROM size: %hu bytes\n", rgbLed->eepromSize);

			// Colour
			printf("Current colour:\nRed: %hhu\nGreen: %hhu\nBlue: %hhu\n", rgbLed->rgb.red, rgbLed->rgb.green, rgbLed->rgb.blue);

			// Idle time
			printf("Idle time: %hhu\n", rgbLed->settings.idleTime);
		}

		// Set colour
		if(setColour)
			rgbledctrl_setRGB(rgbLed, &colour);

		// Set idle time
		if(setIdleTime)
			rgbledctrl_setIdleTime(rgbLed, idleTime);

		// Read EEPROM
		if(readEEPROM)
		{
			byte data;
			if(rgbledctrl_eeprom_read(rgbLed, &data, readEEPROMAddress))
				printf("%hhu", data);
		}

		// Write EEPROM
		if(writeEEPROM)
			rgbledctrl_eeprom_write(rgbLed, writeEEPROMData, writeEEPROMAddress);

		// Close USB handle
		rgbledctrl_close(rgbLed);
		rgbLed = NULL;

		printf("Device closed (Index: %u)\n", i);
	}

	return EXIT_SUCCESS;
}

static void incArgIdx(int* argIdx, int argc)
{
	if(++*argIdx >= argc)
	{
		puts("Error: Bad/missing arguments");
		exit(EXIT_FAILURE);
	}
}

static int getArgVal(int* argIdx, int argc, char **argv, int max)
{
	incArgIdx(argIdx, argc);
	int tmp = atoi(argv[*argIdx]);
	if(max && tmp > max)
		tmp = max;
	return tmp;
}
