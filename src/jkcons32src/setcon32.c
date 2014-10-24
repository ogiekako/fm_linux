// inicon32.c -- initialyze Win32API console control data
// Copyright(c) KAMINA, Jiro 1997-2003.  All rights reserved.
// $Header: inicon32.cv  1.3  03/11/27 10:40:09  Exp $

#include <windows.h>
#include <wincon.h>
#include <stdio.h>

#include "cons32.h"

static char copyright[] =
	"init_console_win32(): "
	"COPYRIGHT(c) KAMINA, Jiro 1997-2003 "
	"- $Header: inicon32.cv  1.3  03/11/27 10:40:09  Exp $"
;

struct curpos_stack *curpos_stack_top = NULL;
HANDLE hStdin, hStdout, hStderr;

static DWORD fdwOldStdinMode;

void
init_console_win32(void)
{
	DWORD fdwMode;

	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "init_console_win32(): GetStdHandle() error - %d\n",
			GetLastError());
		exit(6);
	}
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "init_console_win32(): GetStdHandle() error - %d\n",
			GetLastError());
		exit(6);
	}
	hStderr = GetStdHandle(STD_ERROR_HANDLE);
	if (hStderr == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "init_console_win32(): GetStdHandle() error - %d\n",
			GetLastError());
		exit(6);
	}
	if (!GetConsoleMode(hStdin, &fdwOldStdinMode)) {
		fprintf(stderr, "init_console_win32(): GetConsoleMode() error - %d\n",
			GetLastError());
		exit(6);
	}
	fdwMode = fdwOldStdinMode;
	fdwMode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT);
	if (!SetConsoleMode(hStdin, fdwMode)) {
		fprintf(stderr, "init_console_win32(): SetConsoleMode() error - %d\n",
			GetLastError());
		exit(6);
	}
}

void
reset_console_win32(void)
{
	if (!SetConsoleMode(hStdin, fdwOldStdinMode)) {
		fprintf(stderr, "reset_console_win32(): SetConsoleMode() error - %d\n",
			GetLastError());
		exit(6);
	}
}

