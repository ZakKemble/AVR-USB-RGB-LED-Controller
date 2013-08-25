/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#ifndef DEVICE_H_
#define DEVICE_H_

#include <rgbledctrl.h>
#include "typedefs.h"

void device_init(void);
void device_end(void);
bool device_open(void);
void device_close(void);
bool device_valid(void);
bool device_poke(bool);
void device_reset(void);
void device_setMode(byte);
bool device_setColour(s_rgbVal*);
void device_setTransitionTime(uint16_t);
void device_setBrightness(byte);
void device_setIdleTime(byte);
bool device_eeprom_write(byte, eepAddr_t);
bool device_eeprom_read(byte*, eepAddr_t);
s_device* device_get(void);

#endif /* DEVICE_H_ */
