/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include "common.h"
#include "usb.h"
#include "usbLedController.h"
#include "settings.h"
#include "leds.h"
#include "eeprom.h"
#include "usbdrv/usbdrv.h"

#define USB_REQ_POKE		0
#define USB_REQ_DEVSTATE	1
#define USB_REQ_RESET		2
#define USB_REQ_LED_RGB		3
#define USB_REQ_LED_R		4
#define USB_REQ_LED_G		5
#define USB_REQ_LED_B		6
#define USB_REQ_IDLETIME	7
#define USB_REQ_EEP_WRITE	8
#define USB_REQ_EEP_READ	9

static const byte eepCalibration EEMEM = 0xFF;

static void getDeviceState(void);
static void calibrateOscillator(void);

USB_PUBLIC usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	resetIdleTimer();

	usbRequest_t* rq = (usbRequest_t*)data;
	switch(rq->bRequest)
	{
		case USB_REQ_LED_RGB:
			leds_setColour(rq->wValue.bytes[0], rq->wValue.bytes[1], rq->wIndex.bytes[0]);
			settings_startTimer(SAVE_RGB);
			break;
		case USB_REQ_LED_R:
		case USB_REQ_LED_G:
		case USB_REQ_LED_B:
			{
				byte val = rq->wValue.bytes[0];
				s_rgbVal* colour = leds_colour();
				switch(rq->bRequest)
				{
					case USB_REQ_LED_R:
						colour->red = val;
						break;
					case USB_REQ_LED_G:
						colour->green = val;
						break;
					case USB_REQ_LED_B:
						colour->blue = val;
						break;
				}
				leds_apply();
				settings_startTimer(SAVE_RGB);
			}
			break;
		case USB_REQ_POKE:
			break;
		case USB_REQ_IDLETIME:
			settings_get()->idleTime = rq->wValue.bytes[0];
			settings_startTimer(SAVE_SETTINGS);
			break;
		case USB_REQ_EEP_WRITE:
			eeprom_write(rq->wValue.bytes[0], rq->wIndex.word);
			break;
		case USB_REQ_EEP_READ:
			usbMsgPtr = (usbMsgPtr_t)eeprom_read(rq->wIndex.word);
			return 1;
		case USB_REQ_DEVSTATE:
			getDeviceState();
			return 8;
		case USB_REQ_RESET:
			requestReset();
			break;
		default:
			break;
	}

    return 0;
}

static void getDeviceState()
{
	s_settings* settings = settings_get();
	s_rgbVal* colour = leds_colour();
	static byte stateData[8] = {USB_CFG_DEVICE_VERSION, EEPROM_USER_SIZE>>8, (byte)EEPROM_USER_SIZE};
	stateData[4] = settings->idleTime;
	stateData[5] = colour->red;
	stateData[6] = colour->green;
	stateData[7] = colour->blue;
	usbMsgPtr = (usbMsgPtr_t)stateData;
}






/* Name: osccal.c
 * Author: Christian Starkjohann
 * Creation Date: 2008-04-10
 * Tabsize: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 */

/* ------------------------------------------------------------------------- */
/* ------------------------ Oscillator Calibration ------------------------- */
/* ------------------------------------------------------------------------- */

/* Calibrate the RC oscillator to 8.25 MHz. The core clock of 16.5 MHz is
 * derived from the 66 MHz peripheral clock by dividing. Our timing reference
 * is the Start Of Frame signal (a single SE0 bit) available immediately after
 * a USB RESET. We first do a binary search for the OSCCAL value and then
 * optimize this value with a neighboorhod search.
 * This algorithm may also be used to calibrate the RC oscillator directly to
 * 12 MHz (no PLL involved, can therefore be used on almost ALL AVRs), but this
 * is wide outside the spec for the OSCCAL value and the required precision for
 * the 12 MHz clock! Use the RC oscillator calibrated to 12 MHz for
 * experimental purposes only!
 */
static void calibrateOscillator()
{
	uchar step = 128;
	uchar trialValue = 0, optimumValue;
	int x, optimumDev, targetValue = (unsigned)(1499 * (double)F_CPU / 10.5e6 + 0.5);

    /* do a binary search: */
    do
	{
        OSCCAL = trialValue + step;
        x = usbMeasureFrameLength();    /* proportional to current real frequency */
        if(x < targetValue)             /* frequency still too low */
            trialValue += step;
        step >>= 1;
    }
	while(step > 0);
    /* We have a precision of +/- 1 for optimum OSCCAL here */
    /* now do a neighborhood search for optimum value */
    optimumValue = trialValue;
    optimumDev = x; /* this is certainly far away from optimum */
    for(OSCCAL = trialValue - 1; OSCCAL <= trialValue + 1; OSCCAL++)
	{
        x = usbMeasureFrameLength() - targetValue;
        if(x < 0)
            x = -x;
        if(x < optimumDev)
		{
            optimumDev = x;
            optimumValue = OSCCAL;
        }
    }
    OSCCAL = optimumValue;
}
/*
Note: This calibration algorithm may try OSCCAL values of up to 192 even if
the optimum value is far below 192. It may therefore exceed the allowed clock
frequency of the CPU in low voltage designs!
You may replace this search algorithm with any other algorithm you like if
you have additional constraints such as a maximum CPU clock.
For version 5.x RC oscillators (those with a split range of 2x128 steps, e.g.
ATTiny25, ATTiny45, ATTiny85), it may be useful to search for the optimum in
both regions.
*/

void usbEventResetReady()
{
	cli();
	calibrateOscillator();
	sei();
	eeprom_update_byte((byte*)&eepCalibration, OSCCAL);   /* store the calibrated value in EEPROM */
}

void loadUSBOscCalibration()
{
	byte calibrationValue;
	calibrationValue = eeprom_read_byte(&eepCalibration); /* calibration value from last time */
	if(calibrationValue != 0xff)
		OSCCAL = calibrationValue;
}
