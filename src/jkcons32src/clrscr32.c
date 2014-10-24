/*
 * clrscr32.c -- clear screen
 * Copyright(c) KAMINA, Jiro 1997-2003.  All rights reserved.
 * $Header: clrscr32.cv  1.2  03/11/27 12:31:38  Exp $
*/

#include <stdio.h>
#include <windows.h>
#include <wincon.h>

#include "cons32.h"

static char copyright[] =
	"clear_screen_win32(): "
	"COPYRIGHT(c) KAMINA, Jiro 1997-2003 "
	"- $Header: clrscr32.cv  1.2  03/11/27 12:31:38  Exp $"
;

static CONSOLE_SCREEN_BUFFER_INFO CSBufInfo;
static COORD CurPos;

void
clear_screen_win32(HANDLE hOut)
{
	DWORD Written;
	SHORT Y;

	if (!GetConsoleScreenBufferInfo(hOut, &CSBufInfo)) {
		fprintf(stderr,
			"clear_screen_win32(): GetConsoleScreenBufferInfo() error - %d\n",
			GetLastError());
		exit(6);
	}
	Y = CSBufInfo.srWindow.Bottom - CSBufInfo.srWindow.Top + 1;
	CurPos.X = 0;
	CurPos.Y = 0;
	Written = 0;
	if (!FillConsoleOutputAttribute(hOut,
		//FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
		CSBufInfo.wAttributes, CSBufInfo.dwSize.X * Y, CurPos, &Written)) {
		fprintf(stderr,
			"clear_screen_win32(): FillConsoleOutputAttribute() error - %d\n",
			GetLastError());
		exit(6);
	}
	CurPos.X = 0;
	CurPos.Y = 0;
	Written = 0;
	if (!FillConsoleOutputCharacter(hOut, ' ', CSBufInfo.dwSize.X * Y,
		CurPos, &Written))
	{
		fprintf(stderr,
			"clear_screen_win32(): FillConsoleOutputCharacter() error - %d\n",
			GetLastError());
		exit(6);
	}
	CurPos.X = 0;
	CurPos.Y = 0;
	if (!SetConsoleCursorPosition(hOut, CurPos)) {
		fprintf(stderr,
			"clear_screen_win32(): SetConsoleScreenBufferInfo() error - %d\n",
			GetLastError());
		exit(6);
	}
}

