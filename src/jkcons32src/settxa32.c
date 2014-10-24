/*
 * settxa32.c -- set console text attribute
 * Copyright(c) KAMINA, Jiro 1997-2003.  All rights reserved.
 * $Header: settxa32.cv  1.2  03/11/27 12:22:49  Exp $
*/

#include <windows.h>
#include <wincon.h>
#include <stdio.h>

#include "cons32.h"

static char copyright[] =
	"set_text_attrib_win32(), get_text_attrib_win32(): "
	"COPYRIGHT(c) KAMINA, Jiro 1997-2003 "
	"- $Header: settxa32.cv  1.2  03/11/27 12:22:49  Exp $"
;

void
set_text_attrib_win32(HANDLE hOut, WORD attrib)
{
	if (!SetConsoleTextAttribute(hOut, attrib)) {
		fprintf(stderr,
			"set_text_attrib_win32(): SetConsoleTextAttribute() error - %d\n",
			GetLastError());
		exit(6);
	}
}

WORD
get_text_attrib_win32(HANDLE hOut)
{
	CONSOLE_SCREEN_BUFFER_INFO CSBufInfo;

	if (!GetConsoleScreenBufferInfo(hOut, &CSBufInfo)) {
		fprintf(stderr,
			"set_text_attrib_win32(): "
			"GetConsoleScreenBufferInfo() error - %d\n", GetLastError());
		exit(6);
	}
	return CSBufInfo.wAttributes;
}

