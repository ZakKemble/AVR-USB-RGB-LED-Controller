/*
 * Project: AVR USB RGB LED Controller
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/
 */

#include "win.h"
#include <stdio.h>
#include <shellapi.h>
#include <commctrl.h>
#include "typedefs.h"
#include "main.h"
#include "backend.h"
#include "actions.h"
//#include "cmdLine.h"
#include "LimitSingleInstance.h"
#include "resource.h"

#define MAINCLASSNAME	TEXT("MainWndClass")
#define MAX_LOADSTRING	100

static NOTIFYICONDATA nid;
static TCHAR szTitle[MAX_LOADSTRING];
static HINSTANCE g_hInstance;
static HWND mainWnd;
static s_dlg settingsDlg;

static BOOL CALLBACK SettingsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_INITDIALOG:
			{
				settingsDlg.dlg = hDlg;
				settingsDlg.visible = false;

				// Small icon
				HANDLE iconSmall = LoadImage(
						g_hInstance,
						MAKEINTRESOURCE(IDI_SMALL),
						IMAGE_ICON,
						GetSystemMetrics(SM_CXSMICON),
						GetSystemMetrics(SM_CYSMICON),
						LR_SHARED
					);
				SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)iconSmall);

				// Large icon
				HANDLE iconLarge = LoadImage(
						g_hInstance,
						MAKEINTRESOURCE(IDI_MAIN),
						IMAGE_ICON,
						GetSystemMetrics(SM_CXICON),
						GetSystemMetrics(SM_CYICON),
						LR_SHARED
					);
				SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)iconLarge);

				// Track bar settings
				SendMessage(GetDlgItem(hDlg, IDCHC_TRACKBAR_BRIGHTNESS), TBM_SETRANGE, TRUE, MAKELONG(0, 255));
				SendMessage(GetDlgItem(hDlg, IDCHC_TRACKBAR_HUE), TBM_SETRANGE, TRUE, MAKELONG(0, 360));
			}
			break;
		case WM_SHOWWINDOW:
			// Update GUI when showing window
			settingsDlg.visible = wParam;
			if(settingsDlg.visible)
			{
				actions_showConnectionStatus();
				actions_showCurrentSettings();
			}
			break;
		case WM_COMMAND:
			{
				int wmId    = LOWORD(wParam);
				//int wmEvent = HIWORD(wParam);
				switch(wmId)
				{
					case IDC_BTN_ABOUT:
						{
							TCHAR aboutStr[MAX_LOADSTRING];
							LoadString(g_hInstance, IDS_ABOUT, aboutStr, MAX_LOADSTRING);
							MessageBox(hDlg, aboutStr, "About", MB_OK | MB_ICONINFORMATION);
						}
						break;
					case IDC_BTN_SGL_APPLY:
						actions_applySingleColour();
						break;
					case IDC_BTN_USAGE_APPLY:
						actions_applyUsageMode();
						break;
					case IDC_BTN_RESET:
						actions_deviceReset();
						break;
					case IDCANCEL: // Hide window when X clicked
						ShowWindow(hDlg, SW_HIDE);
						//EndDialog(hDlg, LOWORD(wParam));
						break;
					case IDC_BTN_EXIT: // Exit program with Exit clicked
						EndDialog(hDlg, LOWORD(wParam));
						SendMessage(mainWnd, WM_CLOSE, 0, 0);
						break;
					default:
						return FALSE;
				}
			}
			break;
		case WM_HSCROLL:
			{
				LRESULT position = SendMessage((HWND)lParam, TBM_GETPOS, 0, 0);
				if((HWND)lParam == GetDlgItem(hDlg, IDCHC_TRACKBAR_BRIGHTNESS))
				{
					// Brightness slider
					actions_setItemInt(IDC_EDIT_USAGE_BRT_MAX, position);
					actions_setUsageBrightness();
				}
				else if((HWND)lParam == GetDlgItem(hDlg, IDCHC_TRACKBAR_HUE))
				{
					// Hue slider
					actions_getColourHue(position);
					actions_applySingleColour();
				}
				else
					return FALSE;
			}
	        break;
		case WM_UPDATE_TRACKBAR: // Set trackbar position
			SendMessage(GetDlgItem(hDlg, IDCHC_TRACKBAR_BRIGHTNESS), TBM_SETPOS, TRUE, GetDlgItemInt(hDlg, IDC_EDIT_USAGE_BRT_MAX, NULL, FALSE));
			break;
		case WM_TRAYMESSAGE:
			SendMessage(mainWnd, WM_TRAYMESSAGE, wParam, lParam);
			break;
		case WM_DESTROY:
			settingsDlg.dlg = NULL;
			break;
		default:
			return FALSE;
	}
	return TRUE;
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
		case WM_CREATE:
			{
				settingsDlg.dlg = NULL;
				mainWnd = hWnd;

				// Tray icon
				nid.cbSize = sizeof(NOTIFYICONDATA);
				nid.hWnd = hWnd;
				nid.uID = 100;
				nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
				//nid.uVersion = NOTIFYICON_VERSION;
				nid.uCallbackMessage = WM_TRAYMESSAGE;
				//nid.hIcon = ;
				wcscpy((WCHAR*)nid.szTip, (WCHAR*)szTitle);

				Shell_NotifyIcon(NIM_ADD, &nid);

				// Update icon
				SendMessage(hWnd, WM_TRAYMESSAGE, false, WM_UPDATEICON);
			}
			break;
		case WM_TRAYMESSAGE:
			switch(lParam)
			{
				case WM_LBUTTONDBLCLK: // Tray icon double click
					{
						if(settingsDlg.dlg != NULL)
						{
							ShowWindow(settingsDlg.dlg, SW_SHOWNORMAL);
							SetForegroundWindow(settingsDlg.dlg);
						}
						else
							DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_SETTINGSDLG), hWnd, SettingsDlgProc);
					}
					break;
				case WM_UPDATEICON: // Update tray icon image
					{
						static LPTSTR iconOn = MAKEINTRESOURCE(IDI_SMALL_ON);
						static LPTSTR iconOff = MAKEINTRESOURCE(IDI_SMALL_OFF);
						bool connected = wParam;
						nid.hIcon = LoadImage(
								g_hInstance,
								connected ? iconOn : iconOff,
								IMAGE_ICON,
								0,
								0,
								LR_SHARED
							);
						Shell_NotifyIcon(NIM_MODIFY, &nid);
					}
					break;
				default:
					return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
			break;
		case WM_CLOSE:
			{
				backend_close();
				Shell_NotifyIcon(NIM_DELETE, &nid);
				DestroyWindow(hWnd);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(WM_QUIT);
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

static int begin(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	g_hInstance = hInstance;

//	cmdLine_load();
/*
	if(cmdLine_isset("help") || cmdLine_isset("?"))
	{
		MessageBox(NULL, "nogui [brightness [brightness] transition [transition time (ms)] idle [idle timeout (s)]] [colour r [red val] g [green val] b [blue val]]\nExample - set colour to purple: colour r 255 g 0 b 255","Help", MB_ICONINFORMATION | MB_OK);
		return EXIT_SUCCESS;
	}
	else if(cmdLine_isset("nogui"))
	{
		backend();
		Sleep(1000);

		int val;
		char out[100];

		if(cmdLine_getVal("brightness", out))
		{
			val = atoi(out);
			if(val > 255)
				val = 255;
			else if(val < 0)
				val = 0;
			byte brightness = (byte)val;
			device_setBrightness(brightness);
		}

		if(cmdLine_getVal("transition", out))
		{
			val = atoi(out);
			if(val > 65535)
				val = 65535;
			else if(val < 0)
				val = 0;
			uint transitionTime = (uint)val;
			device_setTransitionTime(transitionTime);
		}

		if(cmdLine_getVal("idle", out))
		{
			val = atoi(out);
			if(val > 255)
				val = 255;
			else if(val < 0)
				val = 0;
			byte idleTime = (byte)val;
			device_setIdleTime(idleTime);
		}

		backend_wait();
		return EXIT_SUCCESS;
	}
*/
/*	if(cmdLine_isset("nogui"))
	{
		backend();
		backend_wait();
		return EXIT_SUCCESS;
	}

	cmdLine_close();
*/
	InitCommonControls();

	HANDLE iconSmall = LoadImage(
			g_hInstance,
			MAKEINTRESOURCE(IDI_SMALL),
			IMAGE_ICON,
			GetSystemMetrics(SM_CXSMICON),
			GetSystemMetrics(SM_CYSMICON),
			LR_SHARED
		);

	HANDLE iconLarge = LoadImage(
			g_hInstance,
			MAKEINTRESOURCE(IDI_MAIN),
			IMAGE_ICON,
			GetSystemMetrics(SM_CXICON),
			GetSystemMetrics(SM_CYICON),
			LR_SHARED
		);

	HANDLE cursor = LoadImage(
			NULL,
			MAKEINTRESOURCE(OCR_NORMAL),
			IMAGE_CURSOR,
			GetSystemMetrics(SM_CXCURSOR),
			GetSystemMetrics(SM_CYCURSOR),
			LR_SHARED
		);

	WNDCLASSEX wcex		= {0};
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.lpfnWndProc	= WndProc;
	wcex.hInstance		= hInstance;
	wcex.lpszClassName	= MAINCLASSNAME;
	wcex.hIcon			= iconLarge;
	wcex.hIconSm		= iconSmall;
	wcex.hCursor		= cursor;

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, "Call to RegisterClassEx failed!", szTitle, MB_ICONEXCLAMATION | MB_OK);
		return EXIT_FAILURE;
	}

	// Create your main window
	HWND hwndMain = CreateWindowEx(0, MAINCLASSNAME, szTitle, WS_OVERLAPPEDWINDOW,
								CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, HWND_DESKTOP, NULL,
								hInstance, NULL);

	if(!hwndMain)
	{
		MessageBox(NULL, "Call to CreateWindowEx failed!", szTitle, MB_ICONEXCLAMATION | MB_OK);
		return EXIT_FAILURE;
	}

	// Show main window
	//ShowWindow(hwndMain, nCmdShow);
	//UpdateWindow(hwndMain);

	//ShowWindow(GetConsoleWindow(), SW_SHOW);

	backend();
	actions(&settingsDlg);

	DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_SETTINGSDLG), hwndMain, SettingsDlgProc);

	// Run the main message loop
	BOOL bRetVal;
	MSG msg;
	while((bRetVal = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRetVal != -1)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			MessageBox(NULL, "Error encountered in message loop!", szTitle, MB_ICONEXCLAMATION | MB_OK);
			return EXIT_FAILURE;
		}
	}

	return (int)msg.wParam;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int exitCode;

	LoadString(hInstance, IDS_APPNAME, szTitle, MAX_LOADSTRING);

	// Only run 1 at a time
	LimitSingleInstance();
	if(IsAnotherInstanceRunning())
	{
		MessageBox(NULL, "Another instance is already running.", szTitle, MB_ICONERROR | MB_OK);
		exitCode = EXIT_FAILURE;
	}
	else
		exitCode = begin(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	LimitSingleInstance_close();

	return exitCode;
}
