/*
 * putcon32.c -- put data on console
 * Copyright(c) KAMINA, Jiro 1997-2002.  All rights reserved.
 * $Header: putcon32.cv  1.2  03/11/27 12:24:06  Exp $
*/

#include <windows.h>
#include <wincon.h>
#include <stdio.h>

static char copyright[] =
	"put_console_win32(): COPYRIGHT(c) KAMINA, Jiro 1997-2002 "
	"- $Header: putcon32.cv  1.2  03/11/27 12:24:06  Exp $"
;

int
put_console_win32(HANDLE hOut, const char *s)
{
	DWORD wc;

	wc = 0;
	if (!WriteConsole(hOut, s, lstrlen(s), &wc, NULL)) {
		fprintf(stderr, "put_console_win32(): WriteConsole() error - %d\n",
			GetLastError());
		exit(6);
	}
	return wc;
}

int
put_console_char_win32(HANDLE hOut, int c)
{
	DWORD wc;
	char buf[1];

	wc = 0;
	buf[0] = c;
	if (!WriteConsole(hOut, buf, 1, &wc, NULL)) {
		fprintf(stderr,
			"put_console_char_win32(): WriteConsole() error - %d\n",
			GetLastError());
		exit(6);
	}
	return wc;
}

