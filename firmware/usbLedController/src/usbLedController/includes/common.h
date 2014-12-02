/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdbool.h>
#include <string.h>

#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <util/delay.h>

#include "config.h"
#include "util.h"
#include "typedefs.h"

#include "wdt.h"

#include "usb.h"
#include "usbLedController.h"
#include "settings.h"
#include "leds.h"
#include "eeprom.h"

#include "../usbdrv/usbdrv.h"

#endif /* COMMON_H_ */