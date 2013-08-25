/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#ifndef ACTIONS_H_
#define ACTIONS_H_

#include "typedefs.h"

void actions(s_dlg*);
void actions_applyUsageMode(void);
void actions_applySingleColour(void);
void actions_deviceReset(void);
void actions_setUsageBrightness(void);
void actions_setTransitionTime(void);
void actions_setIdleTime(void);
void actions_showCurrentSettings(void);
void actions_showConnectionStatus(void);
void actions_showCPULoad(byte);
void actions_setItemInt(int, uint);
void actions_getColourHue(short);

#endif /* ACTIONS_H_ */
