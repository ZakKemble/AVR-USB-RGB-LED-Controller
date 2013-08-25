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

			// Red
			colour.red = getArgVal(&argIdx, argc, argv, 255);

			// Green
			colour.green = getArgVal(&argIdx, argc, argv, 255);

			// Blue
			colour.blue = getArgVal(&argIdx, argc, argv, 255);
		}
		else if(strcmp(argv[argIdx], "idletime") == 0)
		{
			setIdleTime = true;

			// Time
			idleTime = getArgVal(&argIdx, argc, argv, 255);
		}
		else if(strcmp(argv[argIdx], "eeprom") == 0)
		{
			incArgIdx(&argIdx, argc);
			if(strcmp(argv[argIdx], "r") == 0)
			{
				readEEPROM = true;

				// Address
				readEEPROMAddress = getArgVal(&argIdx, argc, argv, 0);
			}
			else if(strcmp(argv[argIdx], "w") == 0)
			{
				writeEEPROM = true;

				// Address
				writeEEPROMAddress = getArgVal(&argIdx, argc, argv, 0);

				// Data
				writeEEPROMData = getArgVal(&argIdx, argc, argv, 255);
			}
			else
			{
				fprintf(stderr, "Unknown EEPROM operation: %s\n", argv[argIdx]);
				exit(EXIT_FAILURE);
			}
		}
	}

	rgbledctrl_init();
	if(rgbledctrl_open()) // Try opening USB handle
	{
		// Get pointer to state, which includes things like firmware version and user EEPROM size
		s_rgbled_device* device = rgbledctrl_getDevice();

		if(getInfo)
		{
			// Version
			printf("Firmware version: %hhu.%hhu\n", device->version[0], device->version[1]);

			// User EEPROM size
			printf("User EEPROM size: %hu bytes\n", device->eepromSize);

			// Colour
			printf("Current colour:\nRed: %hhu\nGreen: %hhu\nBlue: %hhu\n", device->rgb.red, device->rgb.green, device->rgb.blue);

			// Idle time
			printf("Idle time: %hhu\n", device->settings.idleTime);
		}

		// Set colour
		if(setColour)
			rgbledctrl_setRGB(&colour);

		// Set idle time
		if(setIdleTime)
			rgbledctrl_setIdleTime(idleTime);

		// Read EEPROM
		if(readEEPROM)
		{
			if(readEEPROMAddress < device->eepromSize)
			{
				byte data;
				rgbledctrl_eeprom_read(&data, readEEPROMAddress);
				printf("%hhu", data);
			}
			else
				fprintf(stderr, "EEPROM Read address out of range\n");
		}

		// Write EEPROM
		if(writeEEPROM)
		{
			if(writeEEPROMAddress < device->eepromSize)
				rgbledctrl_eeprom_write(writeEEPROMData, writeEEPROMAddress);
			else
				fprintf(stderr, "EEPROM Write address out of range\n");
		}

		// Close USB handle
		rgbledctrl_close();
	}
	else // Failed to open
	{
		fprintf(stderr, "Could not open device\n");
		exit(EXIT_FAILURE);
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
