// clrln32.c -- clear line
// Copyright(c) KAMINA, Jiro 1997-2003.  All rights reserved.
// $Header: clrln32.cv  1.2  03/11/27 12:32:04  Exp $

#include <stdio.h>
#include <windows.h>
#include <wincon.h>

#include "cons32.h"

static char copyright[] =
	"clear_line_win32(), clear_line_wrapped_win32(): "
	"COPYRIGHT(c) KAMINA, Jiro 1997-2003 "
	"- $Header: clrln32.cv  1.2  03/11/27 12:32:04  Exp $"
;

static CONSOLE_SCREEN_BUFFER_INFO CSBufInfo;
static COORD CurPos;

void
clear_line_win32(HANDLE hOut)
{
	DWORD Written;

	if (!GetConsoleScreenBufferInfo(hOut, &CSBufInfo)) {
		fprintf(stderr,
			"clear_line_win32(): GetConsoleScreenBufferInfo() error - %d\n",
			GetLastError());
		exit(6);
	}
	CurPos.X = 0;
	CurPos.Y = CSBufInfo.dwCursorPosition.Y;
	Written = 0;
	if (!FillConsoleOutputAttribute(hOut,
		//FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
		CSBufInfo.wAttributes, CSBufInfo.dwSize.X, CurPos, &Written))
	{
		fprintf(stderr,
			"clear_line_win32(): FillConsoleOutputAttribute() error - %d\n",
			GetLastError());
		exit(6);
	}
	CurPos.X = 0;
	CurPos.Y = CSBufInfo.dwCursorPosition.Y;
	Written = 0;
	if (!FillConsoleOutputCharacter(hOut, ' ', CSBufInfo.dwSize.X, CurPos,
		&Written))
	{
		fprintf(stderr,
			"clear_line_win32(): FillConsoleOutputCharacter() error - %d\n",
			GetLastError());
		exit(6);
	}
	CurPos.X = 0;
	CurPos.Y = CSBufInfo.dwCursorPosition.Y;
	if (!SetConsoleCursorPosition(hOut, CurPos)) {
		fprintf(stderr,
			"clear_line_win32(): SetConsoleScreenBufferInfo() error - %d\n",
			GetLastError());
		exit(6);
	}
}

void
clear_line_wrapped_win32(HANDLE hOut, int lines)
{
	DWORD Written;
	DWORD len;

	if (!GetConsoleScreenBufferInfo(hOut, &CSBufInfo)) {
		fprintf(stderr,
			"clear_line_wrapped_win32(): "
			"GetConsoleScreenBufferInfo() error - %d\n", GetLastError());
		exit(6);
	}
	len = CSBufInfo.dwSize.X * lines;
	CurPos.X = 0;
	CurPos.Y = CSBufInfo.dwCursorPosition.Y;
	Written = 0;
	if (!FillConsoleOutputAttribute(hOut,
		//FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
		CSBufInfo.wAttributes, len, CurPos, &Written))
	{
		fprintf(stderr,
			"clear_line_wrapped_win32(): "
			"FillConsoleOutputAttribute() error - %d\n", GetLastError());
		exit(6);
	}
	CurPos.X = 0;
	CurPos.Y = CSBufInfo.dwCursorPosition.Y;
	Written = 0;
	if (!FillConsoleOutputCharacter(hOut, ' ', len, CurPos, &Written)) {
		fprintf(stderr,
			"clear_line_wrapped_win32(): "
			"FillConsoleOutputCharacter() error - %d\n", GetLastError());
		exit(6);
	}
	CurPos.X = 0;
	CurPos.Y = CSBufInfo.dwCursorPosition.Y;
	if (!SetConsoleCursorPosition(hOut, CurPos)) {
		fprintf(stderr,
			"clear_line_wrapped_win32(): "
			"SetConsoleScreenBufferInfo() error - %d\n", GetLastError());
		exit(6);
	}
}

