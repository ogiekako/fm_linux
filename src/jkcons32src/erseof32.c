/*
 * erseof32.c -- erase end-of-field
 * Copyright(c) KAMINA, Jiro 1997-2003.  All rights reserved.
 * $Header: erseof32.cv  1.2  03/11/27 12:32:53  Exp $
*/

#include <stdio.h>
#include <windows.h>
#include <wincon.h>

#include "cons32.h"

static char copyright[] =
	"erase_eof_win32(), erase_eof_wrapped_win32(): "
	"COPYRIGHT(c) KAMINA, Jiro 1997-2003 "
	"- $Header: erseof32.cv  1.2  03/11/27 12:32:53  Exp $"
;

static CONSOLE_SCREEN_BUFFER_INFO CSBufInfo;
static COORD CurPos;

void
erase_eof_win32(HANDLE hOut)
{
	DWORD Written;

	if (!GetConsoleScreenBufferInfo(hOut, &CSBufInfo)) {
		fprintf(stderr,
			"erase_eof_win32(): GetConsoleScreenBufferInfo() error - %d\n",
			GetLastError());
		exit(6);
	}
	CurPos.X = CSBufInfo.dwCursorPosition.X;
	CurPos.Y = CSBufInfo.dwCursorPosition.Y;
	Written = 0;
	if (!FillConsoleOutputAttribute(hOut,
		//FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
		CSBufInfo.wAttributes,
		CSBufInfo.dwSize.X - CSBufInfo.dwCursorPosition.X, CurPos, &Written))
	{
		fprintf(stderr,
			"erase_eof_win32(): FillConsoleOutputAttribute() error - %d\n",
			GetLastError());
		exit(6);
	}
	CurPos.X = CSBufInfo.dwCursorPosition.X;
	CurPos.Y = CSBufInfo.dwCursorPosition.Y;
	Written = 0;
	if (!FillConsoleOutputCharacter(hOut, ' ',
		CSBufInfo.dwSize.X - CSBufInfo.dwCursorPosition.X, CurPos, &Written))
	{
		fprintf(stderr,
			"erase_eof_win32(): FillConsoleOutputCharacter() error - %d\n",
			GetLastError());
		exit(6);
	}
	CurPos.X = CSBufInfo.dwCursorPosition.X;
	CurPos.Y = CSBufInfo.dwCursorPosition.Y;
	if (!SetConsoleCursorPosition(hOut, CurPos)) {
		fprintf(stderr,
			"erase_eof_win32(): SetConsoleScreenBufferInfo() error - %d\n",
			GetLastError());
		exit(6);
	}
}

void
erase_eof_wrapped_win32(HANDLE hOut, int lines)
{
	DWORD Written;
	DWORD len;

	if (!GetConsoleScreenBufferInfo(hOut, &CSBufInfo)) {
		fprintf(stderr,
			"erase_eof_wrapped_win32(): "
			"GetConsoleScreenBufferInfo() error - %d\n", GetLastError());
		exit(6);
	}
	len = CSBufInfo.dwSize.X * lines - CSBufInfo.dwCursorPosition.X;
	CurPos.X = CSBufInfo.dwCursorPosition.X;
	CurPos.Y = CSBufInfo.dwCursorPosition.Y;
	Written = 0;
	if (!FillConsoleOutputAttribute(hOut,
		//FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
		CSBufInfo.wAttributes, len, CurPos, &Written))
	{
		fprintf(stderr,
			"erase_eof_wrapped_win32(): "
			"FillConsoleOutputAttribute() error - %d\n", GetLastError());
		exit(6);
	}
	CurPos.X = CSBufInfo.dwCursorPosition.X;
	CurPos.Y = CSBufInfo.dwCursorPosition.Y;
	Written = 0;
	if (!FillConsoleOutputCharacter(hOut, ' ', len, CurPos, &Written)) {
		fprintf(stderr,
			"erase_eof_wrapped_win32(): "
			"FillConsoleOutputCharacter() error - %d\n", GetLastError());
		exit(6);
	}
	CurPos.X = CSBufInfo.dwCursorPosition.X;
	CurPos.Y = CSBufInfo.dwCursorPosition.Y;
	if (!SetConsoleCursorPosition(hOut, CurPos)) {
		fprintf(stderr,
			"erase_eof_wrapped_win32(): "
			"SetConsoleScreenBufferInfo() error - %d\n", GetLastError());
		exit(6);
	}
}

