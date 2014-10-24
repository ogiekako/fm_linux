// cr32.c -- carriage return
// Copyright(c) KAMINA, Jiro 1997, 1998.  All rights reserved.
// $Header: cr32.cv  1.2  03/11/27 12:31:29  Exp $

#include <stdio.h>
#include <windows.h>
#include <wincon.h>

#include "cons32.h"

static char copyright[] =
	"carriage_return_win32(): "
	"COPYRIGHT(c) KAMINA, Jiro 1997, 1998 "
	"- $Header: cr32.cv  1.2  03/11/27 12:31:29  Exp $"
;

static CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
static COORD dwCurPos;

void
carriage_return_win32(HANDLE hOut)
{
	if (!GetConsoleScreenBufferInfo(hOut, &csbiInfo)) {
		fprintf(stderr,
			"carriage_return_win32(): "
			"GetConsoleScreenBufferInfo() error - %d\n", GetLastError());
		exit(6);
	}
	dwCurPos.X = 0;
	dwCurPos.Y = csbiInfo.dwCursorPosition.Y;
	if (!SetConsoleCursorPosition(hOut, dwCurPos)) {
		fprintf(stderr,
			"carriage_return_win32(): "
			"SetConsoleScreenBufferInfo() error - %d\n", GetLastError());
		exit(6);
	}
}

