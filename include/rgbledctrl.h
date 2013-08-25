/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#ifndef RGBLEDCTRL_H_
#define RGBLEDCTRL_H_

#ifdef _WIN32
#include <lusb0_usb.h>
#else
#include <usb.h>
#endif
#include <stdbool.h>
#include <stdint.h>

typedef unsigned char byte;
typedef uint16_t eepAddr_t;

typedef struct{
	byte red;
	byte green;
	byte blue;
}s_rgbVal;

typedef struct{
	byte idleTime;
}s_rgbled_deviceSettings;

typedef struct{
	usb_dev_handle* handle;
	byte version[2];
	eepAddr_t eepromSize;
	s_rgbVal rgb;
	s_rgbled_deviceSettings settings;
}s_rgbled_device;

void rgbledctrl_init(void);
bool rgbledctrl_open(void);
void rgbledctrl_close(void);
bool rgbledctrl_valid(void);
bool rgbledctrl_poke(bool);
bool rgbledctrl_reset(void);
bool rgbledctrl_setIdleTime(byte);
bool rgbledctrl_setRGB(s_rgbVal*);
bool rgbledctrl_setR(byte);
bool rgbledctrl_setG(byte);
bool rgbledctrl_setB(byte);
s_rgbled_device* rgbledctrl_getDevice(void);

bool rgbledctrl_eeprom_write(byte, eepAddr_t);
bool rgbledctrl_eeprom_read(byte*, eepAddr_t);

#endif /* RGBLEDCTRL_H_ */
