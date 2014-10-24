/*
 * getcls32.c -- get console lines
 * Copyright(c) KAMINA, Jiro 2003.  All rights reserved.
 * $Header$
*/

#include <windows.h>
#include <wincon.h>
#include <stdio.h>

#include "cons32.h"

static char copyright[] =
	"get_console_lines_win32(): COPYRIGHT(c) KAMINA, Jiro 2003 "
	"- $Header$"
;

int
get_console_lines_win32(void)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!GetConsoleScreenBufferInfo(hStdout, &csbi)) {
		fprintf(stderr,
			"get_console_lines_win32(): "
			"GetConsoleScreenBufferInfo() error - %d\n", GetLastError());
		exit(6);
	}
	return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

