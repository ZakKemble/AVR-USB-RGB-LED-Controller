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
#define DEV_ID		0x05DC
#define VEN_NAME	"zakkemble.co.uk"
#define DEV_NAME	"RGBLEDController"

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

#endif /* MCU_H_ */
