/*
 * setttl32.c -- set window-title
 * Copyright(c) KAMINA, Jiro 1997.  All rights reserved.
 * $Header: setttl32.cv  1.2  03/11/27 12:23:16  Exp $
*/

#include <stdio.h>
#include <windows.h>

#include "title32.h"

static char copyright[] =
	"set_title_win32(): "
	"COPYRIGHT(c) KAMINA, Jiro 1997 "
	"- $Header: setttl32.cv  1.2  03/11/27 12:23:16  Exp $"
;

void
set_title_win32(char *title)
{
	if (title != NULL && !SetConsoleTitle(title)) {
		fprintf(stderr, "set_title_win32(): SetConsoleTitle() error - %d\n",
			GetLastError());
		exit(6);
	}
}

