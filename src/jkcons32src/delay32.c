/*
 * delay32.c -- delay
 * Copyright(c) KAMINA, Jiro 1997.  All rights reserved.
 * $Header: delay32.cv  1.2  03/11/27 12:29:27  Exp $
*/

#include <windows.h>

#include "misc32.h"

static char copyright[] =
	"delay_win32(): "
	"COPYRIGHT(c) KAMINA Jiro 1997 "
	"- $Header: delay32.cv  1.2  03/11/27 12:29:27  Exp $"
;

void
delay_win32(unsigned int d)
{
	Sleep(d);
}

