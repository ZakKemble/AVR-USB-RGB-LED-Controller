/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#ifndef RESOURCE_H_
#define RESOURCE_H_

#define ID_SET1 (WM_APP + 0)
#define ID_SET2 (WM_APP + 100)
#define ID_SET3 (WM_APP + 200)
#define ID_SET4 (WM_APP + 300)

// Icons
#define IDI_MAIN						(ID_SET1 + 1)
#define IDI_SMALL						(ID_SET1 + 2)
#define IDI_SMALL_OFF					(ID_SET1 + 3)
#define IDI_SMALL_ON					IDI_SMALL

// Settings dialog
#define IDD_SETTINGSDLG					(ID_SET1 + 4)

// Single colour controls
#define IDC_GRP_SGL                     (ID_SET2 + 1)
#define IDC_STATIC_SGL_RED				(ID_SET2 + 2)
#define IDC_STATIC_SGL_GREEN			(ID_SET2 + 3)
#define IDC_STATIC_SGL_BLUE				(ID_SET2 + 4)
#define IDC_EDIT_SGL_RED				(ID_SET2 + 5)
#define IDC_EDIT_SGL_GREEN				(ID_SET2 + 6)
#define IDC_EDIT_SGL_BLUE				(ID_SET2 + 7)
#define IDC_BTN_SGL_APPLY				(ID_SET2 + 8)

// CPU usage controls
#define IDC_GRP_USAGE                   (ID_SET3 + 1)
//#define IDC_STATIC_USAGE_RED_MIN		(ID_SET3 + 2)
//#define IDC_STATIC_USAGE_RED_MAX		(ID_SET3 + 3)
//#define IDC_STATIC_USAGE_GREEN_MIN		(ID_SET3 + 4)
//#define IDC_STATIC_USAGE_GREEN_MAX		(ID_SET3 + 5)
//#define IDC_STATIC_USAGE_BLUE_MIN		(ID_SET3 + 6)
//#define IDC_STATIC_USAGE_BLUE_MAX		(ID_SET3 + 7)
//#define IDC_EDIT_USAGE_RED_MIN			(ID_SET3 + 8)
//#define IDC_EDIT_USAGE_RED_MAX			(ID_SET3 + 9)
//#define IDC_EDIT_USAGE_GREEN_MIN		(ID_SET3 + 10)
//#define IDC_EDIT_USAGE_GREEN_MAX		(ID_SET3 + 11)
//#define IDC_EDIT_USAGE_BLUE_MIN			(ID_SET3 + 12)
//#define IDC_EDIT_USAGE_BLUE_MAX			(ID_SET3 + 13)
#define IDC_BTN_USAGE_APPLY				(ID_SET3 + 14)
#define IDC_STATIC_USAGE_BRT_MAX		(ID_SET3 + 15)
#define IDC_EDIT_USAGE_BRT_MAX			(ID_SET3 + 16)
#define IDC_EDIT_TRANS_TIME				(ID_SET3 + 17)
#define IDC_STATIC_USAGE_TRANS_TIME		(ID_SET3 + 18)
#define IDC_STATIC_CPU_USAGE			(ID_SET3 + 19)
#define IDC_STATIC_IDLE_TIME			(ID_SET3 + 20)
#define IDC_EDIT_IDLE_TIME				(ID_SET3 + 21)


// General stuff
#define IDC_STATIC_USB_STATUS			(ID_SET4 + 1)
#define IDC_STATIC_USB_FIRMWARE			(ID_SET4 + 2)
#define IDC_BTN_ABOUT					(ID_SET4 + 3)
#define IDC_BTN_EXIT					(ID_SET4 + 4)
//#define IDC_BTN_DEFAULTS				(ID_SET4 + 5)
//#define IDC_BTN_DISENABLE				(ID_SET4 + 6)
#define IDC_BTN_RESET					(ID_SET4 + 7)

#define IDCHC_TRACKBAR_BRIGHTNESS		(ID_SET4 + 8)
#define IDCHC_TRACKBAR_HUE				(ID_SET4 + 9)

// Strings
#define IDS_APPNAME						(ID_SET4 + 10)
#define IDS_ABOUT						(ID_SET4 + 11)

// Custom messages
#define WM_UPDATE_TRACKBAR				(ID_SET4 + 12)
#define WM_TRAYMESSAGE					(ID_SET4 + 13)
#define WM_UPDATEICON					(ID_SET4 + 14)

#endif /* RESOURCE_H_ */
