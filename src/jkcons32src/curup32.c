// curup32.c -- cursor up and down
// Copyright(c) KAMINA, Jiro 1997, 1998.  All rights reserved.
// $Header: curup32.cv  1.2  03/11/27 12:29:56  Exp $

#include <stdio.h>
#include <windows.h>
#include <wincon.h>

#include "cons32.h"

static char copyright[] =
	"cursor_up_win32(), cursor_down_win3(): "
	"COPYRIGHT(c) KAMINA, Jiro 1997, 1998 "
	"- $Header: curup32.cv  1.2  03/11/27 12:29:56  Exp $"
;

static CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
static COORD dwCurPos;

void
cursor_up_win32(HANDLE hOut, int y)
{
	if (!GetConsoleScreenBufferInfo(hOut, &csbiInfo)) {
		fprintf(stderr,
			"cursor_up_win32(): GetConsoleScreenBufferInfo() error - %d\n",
			GetLastError());
		exit(6);
	}
	dwCurPos.X = csbiInfo.dwCursorPosition.X;
	dwCurPos.Y = csbiInfo.dwCursorPosition.Y - y;
	if (dwCurPos.Y < 0) {
		dwCurPos.Y = 0;
	}
	if (!SetConsoleCursorPosition(hOut, dwCurPos)) {
		fprintf(stderr,
			"cursor_up_win32(): SetConsoleCursorPosition() error - %d\n",
			GetLastError());
		exit(6);
	}
}

void
cursor_down_win32(HANDLE hOut, int y)
{
	if (!GetConsoleScreenBufferInfo(hOut, &csbiInfo)) {
		fprintf(stderr,
			"cursor_down_win32(): GetConsoleScreenBufferInfo() error - %d\n",
			GetLastError());
		exit(6);
	}
	dwCurPos.X = csbiInfo.dwCursorPosition.X;
	dwCurPos.Y = csbiInfo.dwCursorPosition.Y + y;
	if (dwCurPos.Y >= csbiInfo.dwSize.Y) {
		dwCurPos.Y = csbiInfo.dwSize.Y - 1;
	}
	if (!SetConsoleCursorPosition(hOut, dwCurPos)) {
		fprintf(stderr,
			"cursor_down_win32(): SetConsoleCursorPosition() error - %d\n",
			GetLastError());
		exit(6);
	}
}

