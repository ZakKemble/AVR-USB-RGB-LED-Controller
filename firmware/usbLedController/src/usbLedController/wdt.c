/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#include "common.h"

#if WDT_ENABLE

#define wdt_int_reset()		(WDTCR |= _BV(WDIE)|_BV(WDE))

static volatile bool resetWDT;
static byte mcusr_mirror __attribute__ ((section (".noinit")));

void get_mcusr(void) __attribute__((naked)) __attribute__((section(".init3")));

void get_mcusr()
{
	mcusr_mirror = MCUSR;
	MCUSR = 0;

	wdt_enable(WDTO_2S);
	wdt_int_reset();
}

#if WDT_DEBUG
bool wdt_wasReset()
{
	return (mcusr_mirror & _BV(WDRF));
}
#endif

void wdt_update()
{
	if(resetWDT)
	{
		wdt_int_reset();
		resetWDT = false;
	}
}

ISR(WDT_vect)
{
	resetWDT = true;
}

#endif

void wdt_forceReset()
{
	cli();
	wdt_enable(WDTO_15MS);
	while(1){}
}