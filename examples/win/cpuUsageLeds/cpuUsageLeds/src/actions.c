/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#include <rgbledctrl.h>
#include <stdio.h>
#include "typedefs.h"
#include <shellapi.h>
#include <commctrl.h>
#include "actions.h"
#include "device.h"
#include "colour.h"
#include "resource.h"
#include "backend.h"
#include "mcu.h"

static s_dlg* settingsDlg;

static void setTrayIcon(bool);
static void updateTrackBar(void);
static uint getItemInt(int);
static bool settingsDlgIsOpen(void);

void actions(s_dlg* _settingsDlg)
{
	settingsDlg = _settingsDlg;
}

void actions_applyUsageMode()
{
	backend_setMode(MODE_CPU_USAGE);
	actions_setUsageBrightness();
	actions_setTransitionTime();
	actions_setIdleTime();
}

// Put into single colour mode and set the colour
void actions_applySingleColour()
{
	uint r = getItemInt(IDC_EDIT_SGL_RED);
	uint g = getItemInt(IDC_EDIT_SGL_GREEN);
	uint b = getItemInt(IDC_EDIT_SGL_BLUE);
	if(r > 255)
		r = 255;
	if(g > 255)
		g = 255;
	if(b > 255)
		b = 255;
	byte red = r;
	byte green = g;
	byte blue = b;

	actions_setItemInt(IDC_EDIT_SGL_RED,	red);
	actions_setItemInt(IDC_EDIT_SGL_GREEN,	green);
	actions_setItemInt(IDC_EDIT_SGL_BLUE,	blue);

	backend_setMode(MODE_SINGLE_COL);
	backend_setColour(red, green, blue);
}

void actions_deviceReset()
{
	backend_deviceReset();
}

// Set brightness
void actions_setUsageBrightness()
{
	uint bright = getItemInt(IDC_EDIT_USAGE_BRT_MAX);
	if(bright > 255)
		bright = 255;
	byte brightness = bright;

	actions_setItemInt(IDC_EDIT_USAGE_BRT_MAX, brightness);

	updateTrackBar();

	backend_setBrightness(brightness);
}

// Set transition time for CPU usage mode
void actions_setTransitionTime()
{
	uint time = getItemInt(IDC_EDIT_TRANS_TIME);
//	if(time > 3600000) // 1 hour
//		time = 3600000;
	if(time > 60000) // 1 min
		time = 60000;
	else if(time == 0)
		time = 1;
	actions_setItemInt(IDC_EDIT_TRANS_TIME, time);
	backend_setTransitionTime(time);
}

// Set idle time for CPU usage mode
void actions_setIdleTime()
{
	uint time = getItemInt(IDC_EDIT_IDLE_TIME);
	if(time > 255)
		time = 255;
	actions_setItemInt(IDC_EDIT_IDLE_TIME, time);
	backend_setIdleTime(time);
}

// Update GUI with current device settings
void actions_showCurrentSettings()
{
	if(!settingsDlgIsOpen() || !device_valid())
		return;

	s_device* device = device_get();

	// Colour
	actions_setItemInt(IDC_EDIT_SGL_RED,	device->rgbled_device->rgb.red);
	actions_setItemInt(IDC_EDIT_SGL_GREEN,	device->rgbled_device->rgb.green);
	actions_setItemInt(IDC_EDIT_SGL_BLUE,	device->rgbled_device->rgb.blue);

	// Idle time
	actions_setItemInt(IDC_EDIT_IDLE_TIME,	device->rgbled_device->settings.idleTime);

	// Brightness
	actions_setItemInt(IDC_EDIT_USAGE_BRT_MAX,	device->brightness);

	updateTrackBar();

	// Transition time
	actions_setItemInt(IDC_EDIT_TRANS_TIME,	device->transitionTime);
}

// Show connection status and firmware version
void actions_showConnectionStatus()
{
	bool valid = device_valid();
	setTrayIcon(valid);

	if(!settingsDlgIsOpen())
		return;

	if(valid)
	{
		s_device* device = device_get();

		SetDlgItemText(settingsDlg->dlg, IDC_STATIC_USB_STATUS, "Connected");

		char buff[18];
		sprintf(buff,"Firmware: %hhu.%hhu", device->rgbled_device->version[0], device->rgbled_device->version[1]);
		SetDlgItemText(settingsDlg->dlg, IDC_STATIC_USB_FIRMWARE, buff);
	}
	else
	{
		SetDlgItemText(settingsDlg->dlg, IDC_STATIC_USB_STATUS, "Disconnected");
		SetDlgItemText(settingsDlg->dlg, IDC_STATIC_USB_FIRMWARE, "Firmware: -.-");
	}
}

// Update tray icon
static void setTrayIcon(bool connected)
{
	SendMessage(settingsDlg->dlg, WM_TRAYMESSAGE, connected, WM_UPDATEICON);
}

// Show CPU load
void actions_showCPULoad(byte cpuUsage)
{
	if(settingsDlgIsOpen())
	{
		char buff[9];
		sprintf(buff,"CPU: %0hhu", (byte)((100 / 255.0) * cpuUsage));
		SetDlgItemText(settingsDlg->dlg, IDC_STATIC_CPU_USAGE, buff);
	}
}

// Update brightness track bar position
static void updateTrackBar()
{
	if(settingsDlgIsOpen())
		SendMessage(settingsDlg->dlg, WM_UPDATE_TRACKBAR, 0, 0);
}

// Get int value of a control
static uint getItemInt(int item)
{
	return GetDlgItemInt(settingsDlg->dlg, item, NULL, FALSE);
}

// Set a control value
void actions_setItemInt(int item, uint num)
{
	char buff[12];
	sprintf(buff, "%u", num);
	SetDlgItemText(settingsDlg->dlg, item, buff);
}

// Is the settings dialog open
static bool settingsDlgIsOpen()
{
	return (settingsDlg != NULL && settingsDlg->dlg != NULL && (settingsDlg->visible || IsWindowVisible(settingsDlg->dlg)));
}

// Track bar position to hue colour
void actions_getColourHue(short position)
{
	byte red;
	byte green;
	byte blue;
	getHue(position, &red, &green, &blue);
	actions_setItemInt(IDC_EDIT_SGL_RED,	red);
	actions_setItemInt(IDC_EDIT_SGL_GREEN,	green);
	actions_setItemInt(IDC_EDIT_SGL_BLUE,	blue);
}
