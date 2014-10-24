/*
 * inscon32.c -- insert string to console
 * Copyright(c) KAMINA, Jiro 1997-2002.  All rights reserved.
 * $Header: inscon32.cv  1.2  03/11/27 09:57:58  Exp $
*/

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <wincon.h>

#include "cons32.h"

static char copyright[] =
	"insert_console_win32(): "
	"COPYRIGHT(c) KAMINA, Jiro 1997-2002 "
	"- $Header: inscon32.cv  1.2  03/11/27 09:57:58  Exp $"
;

static CONSOLE_SCREEN_BUFFER_INFO CSBufInfo;
static SMALL_RECT ScrollRect;
static COORD DestOrg;
static CHAR_INFO Fill;

int
insert_console_win32(HANDLE hOut, char *s)
{
	DWORD wc = 0;
	int len;

	len = strlen(s);
	if (!GetConsoleScreenBufferInfo(hOut, &CSBufInfo)) {
		fprintf(stderr,
			"insert_console_win32(): GetConsoleScreenBufferInfo() error - %d\n",
			GetLastError());
		exit(6);
	}
	ScrollRect.Left = CSBufInfo.dwCursorPosition.X;
	ScrollRect.Top = CSBufInfo.dwCursorPosition.Y;
	ScrollRect.Right = CSBufInfo.srWindow.Right;
	ScrollRect.Bottom = CSBufInfo.dwCursorPosition.Y;
	DestOrg.X = CSBufInfo.dwCursorPosition.X + len;
	DestOrg.Y = CSBufInfo.dwCursorPosition.Y;
	Fill.Char.AsciiChar = ' ';
	//Fill.Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	Fill.Attributes = CSBufInfo.wAttributes;
	if (!ScrollConsoleScreenBuffer(hOut, &ScrollRect, NULL, DestOrg, &Fill)) {
		fprintf(stderr,
			"insert_console_win32(): ScrollConsoleScreenBuffer() error - %d\n",
			GetLastError());
		exit(6);
	}
	wc = 0;
	if (!WriteConsole(hOut, s, len, &wc, NULL)) {
		fprintf(stderr, "insert_console_win32(): WriteConsole() error - %d\n",
			GetLastError());
		exit(6);
	}
	return wc;
}

