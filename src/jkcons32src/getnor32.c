// getnor32.c -- get # of rows
// Copyright(c) KAMINA, Jiro 2003.  All rights reserved.
// $Header: getnor32.cv  1.2  03/11/25 17:30:30  Exp $

#include <windows.h>
#include <wincon.h>
#include <stdio.h>
#include "cons32.h"

static char copyright[] =
	"get_norows_win32(): "
	"COPYRIGHT(c) KAMINA, Jiro 2003 "
	"- $Header: getnor32.cv  1.2  03/11/25 17:30:30  Exp $"
;

int
get_nobufrows_win32(HANDLE hOut)
{
	CONSOLE_SCREEN_BUFFER_INFO CSBufInfo;

	if (!GetConsoleScreenBufferInfo(hOut, &CSBufInfo)) {
		fprintf(stderr,
			"get_nobufrows_win32(): GetConsoleScreenBufferInfo() error - %d\n",
			GetLastError());
		exit(6);
	}
	return CSBufInfo.dwSize.Y;
}

int
get_nodisprows_win32(HANDLE hOut)
{
	CONSOLE_SCREEN_BUFFER_INFO CSBufInfo;

	if (!GetConsoleScreenBufferInfo(hOut, &CSBufInfo)) {
		fprintf(stderr,
			"get_nodisprows_win32(): GetConsoleScreenBufferInfo() error - %d\n",
			GetLastError());
		exit(6);
	}
	return CSBufInfo.srWindow.Bottom - CSBufInfo.srWindow.Top + 1;
}

