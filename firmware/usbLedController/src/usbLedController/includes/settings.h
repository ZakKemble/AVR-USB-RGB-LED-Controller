/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#define IDLETIME_DISABLE	0
#define SAVE_SETTINGS		0
#define SAVE_RGB			1

void settings_init(void);
void settings_update(void);
settings_t* settings_get(void);
void settings_startTimer(byte);

#endif /* SETTINGS_H_ */