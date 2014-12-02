/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#ifndef MCU_H_
#define MCU_H_

// These must match the settings in the uC firmware
#define VEN_ID		0x16C0
#define DEV_ID		0x05DF
#define VEN_NAME	L"zakkemble.co.uk"
#define DEV_NAME	L"RGBLEDController"

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

#endif /* MCU_H_ */
