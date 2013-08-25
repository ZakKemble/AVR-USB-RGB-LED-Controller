/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#ifndef MCU_H_
#define MCU_H_

#define MODE_CPU_USAGE		1
#define MODE_SINGLE_COL		2

#define IDLETIME_DISABLE	0

#define EEPROM_MODE			1
#define EEPROM_BRIGHTNESS	2
#define EEPROM_TRANSTIME	3 // transtime is 2 bytes

#define TRANSITIONTIME_DEFAULT	1500

#endif /* MCU_H_ */
