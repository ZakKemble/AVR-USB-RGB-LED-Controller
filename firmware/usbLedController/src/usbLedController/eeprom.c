/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#include <avr/eeprom.h>
#include "common.h"
#include "eeprom.h"

// EEPROM stuff for reading and writing over USB

static byte eepUserData[EEPROM_USER_SIZE] EEMEM = {0x00};

void eeprom_write(byte data, eepAddr_t address)
{
	if(address < EEPROM_USER_SIZE)
		eeprom_update_byte((void*)((eepAddr_t)&eepUserData + address), data);
//		eeprom_update_block(&data, (void*)((eepAddr_t)&eepUserData + address), 1);
}

byte* eeprom_read(eepAddr_t address)
{
	static byte data;
	if(address < EEPROM_USER_SIZE)
		data = eeprom_read_byte((void*)((eepAddr_t)&eepUserData + address));
//		eeprom_read_block(&data, (void*)((eepAddr_t)&eepUserData + address), 1);
	else
		data = 0;
	return &data;
}