/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */
/*
#include "win.h"
#include <stdlib.h>
#include <shellapi.h>
#include "typedefs.h"
#include "cmdLine.h"

#define STRBUFFLEN 100

static LPWSTR *szArgList;
static int argCount;

void cmdLine_load()
{
	szArgList = CommandLineToArgvW(GetCommandLineW(), &argCount);
	if (szArgList == NULL)
		MessageBox(NULL, "Unable to parse command line", "Error", MB_ICONEXCLAMATION | MB_OK);
}

void cmdLine_close()
{
	LocalFree(szArgList);
}

// See if an argument is set
bool cmdLine_isset(char* arg)
{
	wchar_t widearray[STRBUFFLEN];
	mbstowcs(widearray, arg, STRBUFFLEN);

	for(uint i = 0; i < argCount; i++)
	{
		if(wcscmp(szArgList[i], widearray) == 0)
			return TRUE;
	}
	return FALSE;
}

// Get argument value, return false if not found
bool cmdLine_getVal(char* arg, char* out)
{
	wchar_t widearray[STRBUFFLEN];
	mbstowcs(widearray, arg, STRBUFFLEN);

	for(uint i = 0; i < argCount; i++)
	{
		if(wcscmp(szArgList[i], widearray) == 0 && i < argCount - 1)
		{
			wcstombs(out, szArgList[i + 1], STRBUFFLEN);
			return TRUE;
		}
	}

	return FALSE;
}
*/
