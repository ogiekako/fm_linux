/*
 * getcp32.c -- get cursor position
 * Copyright(c) KAMINA, Jiro 1997, 1998.  All rights reserved.
 * $Header: getcp32.cv  1.2  03/11/27 12:27:37  Exp $
*/

#include <stdio.h>
#include <windows.h>
#include <wincon.h>

#include "cons32.h"

static char copyright[] =
	"get_cursor_position_win32(): "
	"COPYRIGHT(c) KAMINA, Jiro 1997, 1998 "
	"- $Header: getcp32.cv  1.2  03/11/27 12:27:37  Exp $"
;

static CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

void
get_cursor_position_win32(HANDLE hOut, int *y, int *x)
{
	if (!GetConsoleScreenBufferInfo(hOut, &csbiInfo)) {
		fprintf(stderr,
			"get_cursor_position_win32(): "
			"GetConsoleScreenBufferInfo() error - %d\n", GetLastError());
		exit(6);
	}
	*x = csbiInfo.dwCursorPosition.X + 1;
	*y = csbiInfo.dwCursorPosition.Y + 1;
}

