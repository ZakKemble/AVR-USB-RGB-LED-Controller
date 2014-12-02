/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#include "common.h"

static volatile bool timerTick;
static timer_t idle;
static bool resetRequested;

static void init(void);
static void reset(void);
static void checkTimerTick(void);
static void idleTimer(void);
static void flashTest(void);
static void delay_usbpoll(byte);
#if WDT_ENABLE && WDT_DEBUG
static void resetError(void);
#else
#define resetError() ((void)(0))
#endif

int main(void)
{
	init();

	// Get saved colour
	colour_t colour;
	memcpy(&colour, leds_colour(), sizeof(colour_t));

	loadUSBOscCalibration();
	usbDeviceDisconnect();

	if(wdt_wasReset())
		resetError();

	delay(250);

	usbDeviceConnect();
	usbInit();

	sei();

	flashTest();

	// Load back saved colour
	memcpy(leds_colour(), &colour, sizeof(colour_t));
	leds_apply();
	
	resetIdleTimer();

	while(1)
	{
		checkTimerTick();
		usbPoll();
		if(resetRequested)
			reset();
		wdt_update();
	}
}

static void init()
{
	// Frequency
	clock_prescale_set(CPU_DIV);

	// Power
	ACSR = _BV(ACD);
	power_usi_disable();
	power_adc_disable();

	// Timer0
	TCCR0A = _BV(WGM00)|_BV(WGM01)|_BV(COM0A1)|_BV(COM0B1);
	TCCR0B = _BV(CS00)|_BV(CS01);
	TIMSK = _BV(TOIE0);

	// Timer1
	TCCR1 = _BV(CS12)|_BV(CS11)|_BV(CS10);
	GTCCR = _BV(PWM1B)|_BV(COM1B1);

	leds_init();
	settings_init();
}

void resetIdleTimer()
{
	idle.enabled = true;
	idle.counter = 0;
}

void requestReset()
{
	resetRequested = true;
}

static void reset()
{
	delay_usbpoll(100);
	usbDeviceDisconnect();
	wdt_forceReset();
}

static void checkTimerTick()
{
	// timerTick is set to true approx every millisecond
	if(timerTick)
	{
		timerTick = false;
		idleTimer();
		settings_update();
	}
}

// Idle timer
static void idleTimer()
{
	static byte fader;
	settings_t* settings = settings_get();
	if(settings->idleTime != IDLETIME_DISABLE)
	{
		idle.counter++;
		if(idle.enabled && (idle.counter / 1024) > settings->idleTime)
		{
			idle.enabled = false;
			fader = 0;
		}
		else if(!idle.enabled)
		{
			// 4 seconds to fade from max to min
			if(++fader > 15)
			{
				fader = 0;
				colour_t* leds = leds_colour();
				if(leds->r > 0)
					leds->r--;
				if(leds->g > 0)
					leds->g--;
				if(leds->b > 0)
					leds->b--;
				leds_apply();
			}			
		}
	}
}

// Flash each colour when first turning on 
static void flashTest()
{
	for(byte i=0;i<3;++i)
	{
		leds_setColour(255, 0, 0); // Red
		delay_usbpoll(40);
		leds_setColour(0, 255, 0); // Green
		delay_usbpoll(40);
		leds_setColour(0, 0, 255); // Blue
		delay_usbpoll(40);
//		leds_setColour(0, 255, 0); // Green
//		delay_usbpoll(40);
	}
/*
	for(byte i=0;i<5;++i) // White flash
	{
		leds_setColour(255, 255, 255);
		delay_usbpoll(40);
		leds_setColour(0, 0, 0);
		delay_usbpoll(40);
	}
*/
	leds_setColour(0, 0, 0);
	delay_usbpoll(50);
}

// Delay while still polling USB
static void delay_usbpoll(byte ms)
{
	while(ms--)
	{
		while(!timerTick)
			usbPoll();
		timerTick = false;
	}

	wdt_update();
}

#if WDT_ENABLE && WDT_DEBUG
// Flash red if the controller was restarted by the watchdog
static void resetError()
{
	leds_setColour(0, 0, 0);
	for(byte i=0;i<16;i++)
	{
		wdt_reset();
		delay(100);
		leds_setColour(255, 0, 0);
		delay(100);
		leds_setColour(0, 0, 0);
	}
}
#endif

// 16MHz / 64 prescale = 250KHz timer clock
// 250000 / 256 for overflow = ~976 overflows per second
// ~976 state changes per sec = ~500Hz
// Each overflow is just a tiny bit more than a millisecond
// Actually we're clocked at 16.5MHz so each overflow to be a tiny bit less than a millisecond
ISR(TIMER0_OVF_vect)
{
	timerTick = true;
}
