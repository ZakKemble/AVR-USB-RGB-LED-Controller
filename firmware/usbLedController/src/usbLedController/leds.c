/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#include "common.h"

#define LED_RED		B4
#define LED_GREEN	B1
#define LED_BLUE	B0
#define LED_RED_PWM		OCR1B
#define LED_GREEN_PWM	OCR0B
#define LED_BLUE_PWM	OCR0A

static colour_t colour;

static void setRed(void);
static void setGreen(void);
static void setBlue(void);

void leds_init()
{
	pinMode(LED_RED,	OUTPUT);
	pinMode(LED_GREEN,	OUTPUT);
	pinMode(LED_BLUE,	OUTPUT);

	leds_setColour(0, 0, 0);
}

colour_t* leds_colour()
{
	return &colour;
}

void leds_apply()
{
	setRed();
	setGreen();
	setBlue();
}

void leds_setColour(byte r, byte g, byte b)
{
	colour.r = r;
	colour.g = g;
	colour.b = b;
	leds_apply();
}

static void setRed()
{
	// Timer1 is only used for red, turn it off when not needed.

	byte brightness = colour.r;
	if(brightness == 0 || brightness == 255)
	{
		if(brightness == 0)
			pinWrite(LED_RED, LOW);
		else
			pinWrite(LED_RED, HIGH);
		GTCCR &= ~_BV(COM1B1);
		power_timer1_disable();
	}
	else
	{
		power_timer1_enable();
		GTCCR |= _BV(COM1B1);
		LED_RED_PWM = brightness;
	}
}

static void setGreen()
{
	byte brightness = colour.g;
	if(brightness == 0 || brightness == 255)
	{
		if(brightness == 0)
			pinWrite(LED_GREEN, LOW);
		else
			pinWrite(LED_GREEN, HIGH);
		TCCR0A &= ~_BV(COM0B1);
	}
	else
	{
		TCCR0A |= _BV(COM0B1);
		LED_GREEN_PWM = brightness;
	}
}

static void setBlue()
{
	byte brightness = colour.b;
	if(brightness == 0 || brightness == 255)
	{
		if(brightness == 0)
			pinWrite(LED_BLUE, LOW);
		else
			pinWrite(LED_BLUE, HIGH);
		TCCR0A &= ~_BV(COM0A1);
	}
	else
	{
		TCCR0A |= _BV(COM0A1);
		LED_BLUE_PWM = brightness;
	}
}