// erscsb32.c -- erase end-of-console-screen-buffer
// Copyright(c) KAMINA, Jiro 1997-2003.  All rights reserved.
// $Header: erscsb32.cv  1.2  03/11/25 12:58:21  Exp $

#include <stdio.h>
#include <windows.h>
#include <wincon.h>
#include "cons32.h"

static char copyright[] =
	"erase_csb_win32(): "
	"COPYRIGHT(c) KAMINA, Jiro 1997-2003 "
	"- $Header: erscsb32.cv  1.2  03/11/25 12:58:21  Exp $"
;

static CONSOLE_SCREEN_BUFFER_INFO CSBufInfo;
static COORD CurPos;

void
erase_csb_win32(HANDLE hOut)
{
	DWORD Written;
	SHORT Y;

	if (!GetConsoleScreenBufferInfo(hOut, &CSBufInfo)) {
		fprintf(stderr,
			"erase_csb_win32(): GetConsoleScreenBufferInfo() error - %d\n",
			GetLastError());
		exit(6);
	}
	Y = CSBufInfo.dwSize.Y - CSBufInfo.dwCursorPosition.Y;
	CurPos.X = 0;
	CurPos.Y = CSBufInfo.dwCursorPosition.Y;
	Written = 0;
	if (!FillConsoleOutputAttribute(hOut, CSBufInfo.wAttributes,
			CSBufInfo.dwSize.X * Y, CurPos, &Written))
	{
		fprintf(stderr,
			"erase_csb_win32(): FillConsoleOutputAttribute() error - %d\n",
			GetLastError());
		exit(6);
	}
	CurPos.X = 0;
	CurPos.Y = CSBufInfo.dwCursorPosition.Y;
	Written = 0;
	if (!FillConsoleOutputCharacter(hOut, ' ',
			CSBufInfo.dwSize.X * Y, CurPos, &Written))
	{
		fprintf(stderr,
			"erase_csb_win32(): FillConsoleOutputCharacter() error - %d\n",
			GetLastError());
		exit(6);
	}
	CurPos.X = 0;
	CurPos.Y = CSBufInfo.dwCursorPosition.Y;
	if (!SetConsoleCursorPosition(hOut, CurPos)) {
		fprintf(stderr,
			"erase_csb_win32(): SetConsoleScreenBufferInfo() error - %d\n",
			GetLastError());
		exit(6);
	}
}

