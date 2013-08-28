/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>
#include "typedefs.h"
#include "main.h"
#include "backend.h"

static void incArgIdx(int*, int);
static bool alreadyRunning(void);

int main(int argc, char **argv)
{
	puts("CPU Usage LEDs v1.3");
	puts("zakkemble.co.uk");

	bool daemonize = true;
	byte brightness = 255;
	ushort transTime = 1500;

	// Check args
	for(int argIdx = 1;argIdx<argc;argIdx++)
	{
		if(strcmp(argv[argIdx], "-h") == 0 || strcmp(argv[argIdx], "--help") == 0)
		{
			printf("Usage: %s [--help|-h] [-b <brightness>] [-t <transition time>] [-d]", argv[0]);
			puts("");
			puts(" --help|-h            : Help (This)");
			puts(" -b <brightness>      : LED Brightness 0 - 255 (Default 255)");
			puts(" -t <transition time> : Sampling period (Default 1500ms)");
			puts(" -d                   : Don't daemonize (Debugging)");
			exit(EXIT_SUCCESS);
		}
		else if(strcmp(argv[argIdx], "-b") == 0)
		{
			incArgIdx(&argIdx, argc);
			int tmp = atoi(argv[argIdx]);
			if(tmp > 255)
				tmp = 255;
			brightness = tmp;
		}
		else if(strcmp(argv[argIdx], "-t") == 0)
		{
			incArgIdx(&argIdx, argc);
			int tmp = atoi(argv[argIdx]);
			if(tmp > 60000)
				tmp = 60000;
			transTime = tmp;
		}
		else if(strcmp(argv[argIdx], "-d") == 0)
			daemonize = false;
	}

	// Make sure only 1 instance is running
	if(alreadyRunning())
	{
		fputs("Error: Another instance is already running", stderr);
		exit(EXIT_FAILURE);
	}

	// Run in background
	if(daemonize && daemon(0, 0) < 0)
		error(EXIT_FAILURE, errno, "Failed to daemonize");

	backend(brightness, transTime);

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

// Todo...
static bool alreadyRunning()
{
	return false;
}
