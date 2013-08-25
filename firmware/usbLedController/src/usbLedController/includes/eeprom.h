/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include "typedefs.h"

#if defined(__AVR_ATtiny85__)
#define EEPROM_USER_SIZE 507
#elif defined(__AVR_ATtiny45__)
#define EEPROM_USER_SIZE 251
#else
#warning "Unknown device, setting user EEPROM size to 8"
#define EEPROM_USER_SIZE 8
#endif

void eeprom_write(byte, eepAddr_t);
byte* eeprom_read(eepAddr_t);

#endif /* EEPROM_H_ */