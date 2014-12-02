/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#include "common.h"

typedef enum
{
	USB_REP_POKE		= 1,
	USB_REP_DEVINFO		= 2,
	USB_REP_RESET		= 3,
	USB_REP_LED_RGB		= 4,
	USB_REP_LED_R		= 5,
	USB_REP_LED_G		= 6,
	USB_REP_LED_B		= 7,
	USB_REP_IDLETIME	= 8,
	USB_REP_EEP_SETUP	= 9,
	USB_REP_EEP			= 10
}usb_rep_t;

#define VALUE_POKE	241
#define VALUE_RESET	184

#define REPORT_SIZE	3 // Size of largest report

// Host software HIDAPI stuff doesn't seem to like INPUT reports, would stop communicating with device after a few seconds

PROGMEM const char usbHidReportDescriptor[105] = {    /* USB report descriptor */
	0x06, 0x00, 0xff,              // USAGE_PAGE (Vendor Defined Page 1)
	0x09, 0x00,                    // USAGE (Undefined)
	0xa1, 0x01,                    // COLLECTION (Application)
	0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
	0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
	0x75, 0x08,                    //   REPORT_SIZE (8) (Bits per field)

// POKE
	0x95, 0x01,                    //   REPORT_COUNT (1) (Number of fields)
	0x85, USB_REP_POKE,            //   REPORT_ID
	0x09, 0x00,                    //   USAGE (Undefined)
	0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

// DEVINFO
	0x95, 0x04,                    //   REPORT_COUNT
	0x85, USB_REP_DEVINFO,         //   REPORT_ID
	0x09, 0x00,                    //   USAGE (Undefined)
	0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

// RESET
	0x95, 0x01,                    //   REPORT_COUNT (0)
	0x85, USB_REP_RESET,           //   REPORT_ID
	0x09, 0x00,                    //   USAGE (Undefined)
	0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

// LED_RGB
	0x95, 0x03,                    //   REPORT_COUNT (3)
	0x85, USB_REP_LED_RGB,         //   REPORT_ID
	0x09, 0x00,                    //   USAGE (Undefined)
	0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

// LED_RED
	0x95, 0x01,                    //   REPORT_COUNT (1)
	0x85, USB_REP_LED_R,           //   REPORT_ID
	0x09, 0x00,                    //   USAGE (Undefined)
	0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

// LED_GREEN
	0x95, 0x01,                    //   REPORT_COUNT (1)
	0x85, USB_REP_LED_G,           //   REPORT_ID
	0x09, 0x00,                    //   USAGE (Undefined)
	0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

// LED_BLUE
	0x95, 0x01,                    //   REPORT_COUNT (1)
	0x85, USB_REP_LED_B,           //   REPORT_ID (7)
	0x09, 0x00,                    //   USAGE (Undefined)
	0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

// IDLETIME
	0x95, 0x01,                    //   REPORT_COUNT (1)
	0x85, USB_REP_IDLETIME,        //   REPORT_ID
	0x09, 0x00,                    //   USAGE (Undefined)
	0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

// EEP_SETUP
	0x95, 0x02,                    //   REPORT_COUNT (2)
	0x85, USB_REP_EEP_SETUP,       //   REPORT_ID
	0x09, 0x00,                    //   USAGE (Undefined)
	0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

// EEP_WRITE/EEP_READ
	0x95, 0x01,                    //   REPORT_COUNT (1)
	0x85, USB_REP_EEP,             //   REPORT_ID
	0x09, 0x00,                    //   USAGE (Undefined)
	0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
	
	0xc0                           // END_COLLECTION
};

static byte eepCalibration EEMEM = 0xFF;

static void calibrateOscillator(void);

typedef struct{
	byte len; // Report length
	byte idx; // Buffer index
	union
	{
		byte buffer[REPORT_SIZE + 1];
		struct {
			byte id; // Report ID
			byte data[REPORT_SIZE]; // Report data
		};
	};
}reportData_t;

static eepAddr_t eepAddr;
static reportData_t report;

static void processRequest(void)
{
	switch(report.id)
	{
		case USB_REP_LED_RGB:
			leds_setColour(report.data[0], report.data[1], report.data[2]);
			settings_startTimer(SAVE_RGB);
			break;
		case USB_REP_LED_R:
		case USB_REP_LED_G:
		case USB_REP_LED_B:
			{
				byte val = report.data[0];
				colour_t* colour = leds_colour();
				switch(report.id)
				{
					case USB_REP_LED_R:
						colour->r = val;
						break;
					case USB_REP_LED_G:
						colour->g = val;
						break;
					case USB_REP_LED_B:
						colour->b = val;
						break;
					default:
						break;
				}
				leds_apply();
				settings_startTimer(SAVE_RGB);
			}
			break;
		case USB_REP_IDLETIME:
			settings_get()->idleTime = report.data[0];
			settings_startTimer(SAVE_SETTINGS);
			break;
		case USB_REP_EEP_SETUP:
			eepAddr = (report.data[0] | (report.data[1]<<8));
			break;
		case USB_REP_EEP:
			eeprom_write(report.data[0], eepAddr);
			break;
		case USB_REP_RESET:
			if(report.data[0] == VALUE_RESET)
				requestReset();
			break;
		default:
			break;
	}
}

uchar usbFunctionRead(uchar* data, uchar len)
{
	UNUSED(data);
	UNUSED(len);
	return 0;
}

uchar usbFunctionWrite(uchar* data, uchar len)
{
	if(report.idx >= report.len)
		return 1;

	for(byte i=0;i<len;i++)
	{
		report.buffer[report.idx++] = data[i];
		if(report.idx >= report.len)
		{
			processRequest();
			return 1;
		}
	}

	return 0;
}

static usbMsgLen_t processGetReport(usb_rep_t _reportId, byte len)
{
	UNUSED(len);

	switch(_reportId)
	{
		case USB_REP_DEVINFO:
		{
			static byte stateData[4] = {USB_CFG_DEVICE_VERSION, (byte)EEPROM_USER_SIZE, EEPROM_USER_SIZE>>8};
			usbMsgPtr = (usbMsgPtr_t)stateData;
			return sizeof(stateData);
		}
		case USB_REP_LED_RGB:
			usbMsgPtr = (usbMsgPtr_t)leds_colour();
			return sizeof(colour_t);
		case USB_REP_IDLETIME:
			usbMsgPtr = (usbMsgPtr_t)&settings_get()->idleTime;
			return 1;
		case USB_REP_EEP:
			usbMsgPtr = (usbMsgPtr_t)eeprom_read(eepAddr);
			return 1;
		case USB_REP_POKE:
		{
			static byte pokeValue = VALUE_POKE;
			usbMsgPtr = (usbMsgPtr_t)&pokeValue;
			return 1;
		}
		default:
			break;
	}
	return 0;
}

static usbMsgLen_t processSetReport(usb_rep_t _reportId, byte len)
{
	UNUSED(_reportId);

	// len = Report data length + 1 for report ID

	report.len = len;
	report.idx = 0;

	if(report.len > sizeof(report.buffer))
		return 0;

	return USB_NO_MSG;
}

USB_PUBLIC usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	resetIdleTimer();
	
	usbRequest_t* rq = (usbRequest_t*)data;
	if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS)
	{
		if(rq->bRequest == USBRQ_HID_GET_REPORT)
			return processGetReport(rq->wValue.bytes[0], rq->wLength.bytes[0]);
		else if(rq->bRequest == USBRQ_HID_SET_REPORT)
			return processSetReport(rq->wValue.bytes[0], rq->wLength.bytes[0]);
	}

	return 0;
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
