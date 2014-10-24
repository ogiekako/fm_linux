/*
 * curon32.c -- set cursor on/off
 * Copyright(c) KAMINA, Jiro 1997, 1998.  All rights reserved.
 * $Header: curon32.cv  1.2  03/11/27 12:30:22  Exp $
*/

#include <stdio.h>
#include <windows.h>
#include <wincon.h>

#include "cons32.h"

static char copyright[] =
	"cursor_on_win32(), cursor_off_win32(): "
	"COPYRIGHT(c) KAMINA, Jiro 1997, 1998 "
	"- $Header: curon32.cv  1.2  03/11/27 12:30:22  Exp $"
;

static CONSOLE_CURSOR_INFO cciInfo;

void
cursor_on_win32(HANDLE hOut)
{
	if (!GetConsoleCursorInfo(hOut, &cciInfo)) {
		fprintf(stderr,
			"cursor_on_win32(): GetConsoleCursorInfo() error - %d\n",
			GetLastError());
		exit(6);
	}
	cciInfo.bVisible = TRUE;
	if (!SetConsoleCursorInfo(hOut, &cciInfo)) {
		fprintf(stderr,
			"cursor_on_win32(): SetConsoleCursorInfo() error - %d\n",
			GetLastError());
		exit(6);
	}
}

void
cursor_off_win32(HANDLE hOut)
{
	if (!GetConsoleCursorInfo(hOut, &cciInfo)) {
		fprintf(stderr,
			"cursor_on_win32(): GetConsoleCursorInfo() error - %d\n",
			GetLastError());
		exit(6);
	}
	cciInfo.bVisible = FALSE;
	if (!SetConsoleCursorInfo(hOut, &cciInfo)) {
		fprintf(stderr,
			"cursor_on_win32(): SetConsoleCursorInfo() error - %d\n",
			GetLastError());
		exit(6);
	}
}

