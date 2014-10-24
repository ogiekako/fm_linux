/*
 * locate32.c -- set cursor position
 * Copyright(c) KAMINA, Jiro 1997, 1998.  All rights reserved.
 * $Header: locate32.cv  1.2  03/11/27 12:25:49  Exp $
*/

#include <stdio.h>
#include <windows.h>
#include <wincon.h>

#include "cons32.h"

static char copyright[] =
	"locate_win32(): "
	"COPYRIGHT(c) KAMINA, Jiro 1997, 1998 "
	"- $Header: locate32.cv  1.2  03/11/27 12:25:49  Exp $"
;

static COORD dwCurPos;

void
locate_win32(HANDLE hOut, int y, int x)
{
	dwCurPos.X = x - 1;
	dwCurPos.Y = y - 1;
	if (!SetConsoleCursorPosition(hOut, dwCurPos)) {
		fprintf(stderr,
			"locate_win32(): SetConsoleScreenBufferInfo() error - %d\n",
			GetLastError());
		exit(6);
	}
}

