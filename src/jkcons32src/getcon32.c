/*
 * getcon32.c -- get data on console
 * Copyright(c) KAMINA, Jiro 1997.  All rights reserved.
 * $Header: getcon32.cv  1.2  03/11/27 12:28:21  Exp $
*/

#include <windows.h>
#include <wincon.h>
#include <stdio.h>
#include <stdlib.h>

#include "cons32.h"

static char copyright[] =
	"get_console_win32(): COPYRIGHT(c) KAMINA, Jiro 1997 "
	"- $Header: getcon32.cv  1.2  03/11/27 12:28:21  Exp $"
;

int
get_console_win32(HANDLE hIn, LPVOID buf, DWORD rc)
{
	DWORD rc2;

	rc2 = 0;
	if (!ReadConsole(hIn, buf, rc, &rc2, NULL)) {
		fprintf(stderr, "get_console_win32(): ReadConsole() error - %d\n",
			GetLastError());
		exit(6);
	}
	return rc2;
}

int
get_console_char_win32(HANDLE hIn)
{
	DWORD rc2;
	int c;
	INPUT_RECORD inrec;
	static int nib = 0;
	static DWORD cks = 0;
	static char inbuf[256];

	if (nib > 0) {
		goto return_char;
	}

read_again:;
	rc2 = 0;
	if (!ReadConsoleInput(hIn, &inrec, 1, &rc2)) {
		fprintf(stderr, "get_console_win32(): ReadConsoleInput() error - %d\n",
			GetLastError());
		exit(6);
	}
	if (inrec.EventType != KEY_EVENT) {
		goto read_again;
	}
	if (inrec.Event.KeyEvent.bKeyDown == FALSE) {
		goto read_again;
	}
	if (inrec.Event.KeyEvent.uChar.AsciiChar != 0x00) {
		nib = 1;
		inbuf[0] = inrec.Event.KeyEvent.uChar.AsciiChar;
	} else {
		nib = 2;
		inbuf[0] = 0x00;
		inbuf[1] = (char)inrec.Event.KeyEvent.wVirtualScanCode;
	}

return_char:;
	nib--;
	c = inbuf[0];
	memmove(inbuf, inbuf + 1, nib);
	return c;
}

#if 0
		if (inrec.Event.KeyEvent.dwControlKeyState & LEFT_ALT_PRESSED) {
			cks |= LEFT_ALT_PRESSED;
		}
		if (inrec.Event.KeyEvent.dwControlKeyState & LEFT_CTRL_PRESSED) {
			cks |= LEFT_CTRL_PRESSED;
		}
		if (inrec.Event.KeyEvent.dwControlKeyState & RIGHT_ALT_PRESSED) {
			cks |= RIGHT_ALT_PRESSED;
		}
		if (inrec.Event.KeyEvent.dwControlKeyState & RIGHT_CTRL_PRESSED) {
			cks |= RIGHT_CTRL_PRESSED;
		}
		if (inrec.Event.KeyEvent.dwControlKeyState & SHIFT_PRESSED) {
			cks |= SHIFT_PRESSED;
		}
#endif
#if 0
		if (inrec.Event.KeyEvent.dwControlKeyState & LEFT_ALT_PRESSED) {
			cks &= ~LEFT_ALT_PRESSED;
		}
		if (inrec.Event.KeyEvent.dwControlKeyState & LEFT_CTRL_PRESSED) {
			cks &= ~LEFT_CTRL_PRESSED;
		}
		if (inrec.Event.KeyEvent.dwControlKeyState & RIGHT_ALT_PRESSED) {
			cks &= ~RIGHT_ALT_PRESSED;
		}
		if (inrec.Event.KeyEvent.dwControlKeyState & RIGHT_CTRL_PRESSED) {
			cks &= ~RIGHT_CTRL_PRESSED;
		}
		if (inrec.Event.KeyEvent.dwControlKeyState & SHIFT_PRESSED) {
			cks &= ~SHIFT_PRESSED;
		}
#endif
#if 0
	{
		locate_win32(ROW_C, COL_DBG);
		switch (inrec.EventType) {
		case KEY_EVENT:
			strcpy(buf, "KEY_EVENT");
			break;
		case MOUSE_EVENT:
			strcpy(buf, "MOUSE_EVENT");
			break;
		case WINDOW_BUFFER_SIZE_EVENT:
			strcpy(buf, "WINDOW_BUFFER_SIZE_EVENT");
			break;
		case MENU_EVENT:
			strcpy(buf, "MENU_EVENT");
			break;
		case FOCUS_EVENT:
			strcpy(buf, "FOCUS_EVENT");
			break;
		}
		put_console_win32(buf);
	}
#endif
#if 0
	{
		locate_win32(ROW_C, COL_DBG);
		strcpy(buf, (inrec.Event.KeyEvent.bKeyDown == TRUE)? "TRUE": "FALSE");
		if (inrec.Event.KeyEvent.dwControlKeyState & CAPSLOCK_ON) {
			strcat(buf, " CAPSLOCK_ON");
		}
		if (inrec.Event.KeyEvent.dwControlKeyState & ENHANCED_KEY) {
			strcat(buf, " ENHANCED_KEY");
		}
		if (inrec.Event.KeyEvent.dwControlKeyState & LEFT_ALT_PRESSED) {
			strcat(buf, " LEFT_ALT_PRESSED");
		}
		if (inrec.Event.KeyEvent.dwControlKeyState & LEFT_CTRL_PRESSED) {
			strcat(buf, " LEFT_CTRL_PRESSED");
		}
		if (inrec.Event.KeyEvent.dwControlKeyState & NUMLOCK_ON) {
			strcat(buf, " NUMLOCK_ON");
		}
		if (inrec.Event.KeyEvent.dwControlKeyState & RIGHT_ALT_PRESSED) {
			strcat(buf, " RIGHT_ALT_PRESSED");
		}
		if (inrec.Event.KeyEvent.dwControlKeyState & RIGHT_CTRL_PRESSED) {
			strcat(buf, " RIGHT_CTRL_PRESSED");
		}
		if (inrec.Event.KeyEvent.dwControlKeyState & SCROLLLOCK_ON) {
			strcat(buf, " SCROLLLOCK_ON");
		}
		if (inrec.Event.KeyEvent.dwControlKeyState & SHIFT_PRESSED) {
			strcat(buf, " SHIFT_PRESSED");
		}
		put_console_win32(buf);
		erase_eof_win32();
		locate_win32(ROW_D, COL_DBG);
		sprintf(buf, "%d", inrec.Event.KeyEvent.wRepeatCount);
		sprintf(buf + strlen(buf), " 0x%04x",
		  inrec.Event.KeyEvent.wVirtualKeyCode);
		sprintf(buf + strlen(buf), " 0x%04x",
		  inrec.Event.KeyEvent.wVirtualScanCode);
		sprintf(buf + strlen(buf), " 0x%02x",
		  inrec.Event.KeyEvent.uChar.AsciiChar);
		put_console_win32(buf);
		erase_eof_win32();
	}
#endif
#if 0
	if (inrec.Event.KeyEvent.bKeyDown == FALSE
	 || (inrec.Event.KeyEvent.uChar.AsciiChar == 0x00
	  && cks != inrec.Event.KeyEvent.dwControlKeyState)) {
		cks = inrec.Event.KeyEvent.dwControlKeyState;
		goto readagain;
	}
	if (inrec.Event.KeyEvent.uChar.AsciiChar != 0x00) {
		nib = 1;
		inbuf[0] = inrec.Event.KeyEvent.uChar.AsciiChar;
	} else {
		nib = 2;
		inbuf[0] = 0x00;
		inbuf[1] = (char) inrec.Event.KeyEvent.wVirtualScanCode;
	}
	cks = inrec.Event.KeyEvent.dwControlKeyState;
#endif
