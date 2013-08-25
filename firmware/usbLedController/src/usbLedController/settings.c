/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#include <avr/eeprom.h>
#include "common.h"
#include "settings.h"
#include "leds.h"

#define SAVE_DELAY_TIME	4000
#define TIMER_COUNT	2

static const s_settings eepSettings EEMEM = {
	5	// Idle time
};

static const s_rgbVal eepRgbVal EEMEM = {
	0,	// Red
	0,	// Green
	255	// Blue
};

static s_settings settings;
static s_timer eepSave[TIMER_COUNT];

static void load(void);
static void save(byte);

void settings_init()
{
	load();
}

void settings_update()
{
	for(byte i=0;i<TIMER_COUNT;i++)
	{
		if(eepSave[i].enabled)
		{
			eepSave[i].counter++;
			if(eepSave[i].counter > SAVE_DELAY_TIME)
			{
				save(i);
				eepSave[i].enabled = false;
				eepSave[i].counter = 0;
			}
		}	
	}
}

s_settings* settings_get()
{
	return &settings;
}

void settings_startTimer(byte timer)
{
	eepSave[timer].enabled = true;
	eepSave[timer].counter = 0;
}

static void load()
{
	eeprom_read_block(&settings, &eepSettings, sizeof(s_settings));
	eeprom_read_block(leds_colour(), &eepRgbVal, sizeof(s_rgbVal));
}

static void save(byte data)
{
	switch(data)
	{
		case SAVE_SETTINGS:
			eeprom_update_block(&settings, (s_settings*)&eepSettings, sizeof(s_settings));
			break;
		case SAVE_RGB:
			eeprom_update_block(leds_colour(), (s_rgbVal*)&eepRgbVal, sizeof(s_rgbVal));
			break;
	}
}
