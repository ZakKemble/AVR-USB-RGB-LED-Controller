/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#include <stdlib.h>
#include <rgbledctrl.h>
#include <time.h>
#include "flasher.h"

#define UNUSED(var) ((void)(var))

int main(int argc, char **argv)
{
	UNUSED(argc);
	UNUSED(argv);

	// Seed RNG
	srand(time(NULL));

	// Init
	rgbledctrl::init();

	// New flasher
	flasher* Flasher = new flasher();
	Flasher->begin();

	delete Flasher;

	return EXIT_SUCCESS;
}
