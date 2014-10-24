/*
 * bschar32.c -- backspace or delete character
 * Copyright(c) KAMINA, Jiro 1997-2003.  All rights reserved.
 * $Header: bschar32.cv  1.3  03/11/27 15:08:16  Exp $
*/

#include <stdio.h>
#include <windows.h>
#include <wincon.h>

#include "cons32.h"

static char copyright[] =
	"backspace_char_win32(), delete_char_win32(): "
	"COPYRIGHT(c) KAMINA, Jiro 1997-2003 "
	"- $Header: bschar32.cv  1.3  03/11/27 15:08:16  Exp $"
;

static CONSOLE_SCREEN_BUFFER_INFO CSBufInfo;
static SMALL_RECT ScrollRect;
static COORD DestOrg;
static CHAR_INFO Fill;

void
backspace_char_win32(HANDLE hOut)
{
	if (!GetConsoleScreenBufferInfo(hOut, &CSBufInfo)) {
		fprintf(stderr,
			"backspace_char_win32(): GetConsoleScreenBufferInfo() error - %d\n",
			GetLastError());
		exit(6);
	}
	ScrollRect.Left = CSBufInfo.dwCursorPosition.X;
	ScrollRect.Top = CSBufInfo.dwCursorPosition.Y;
	ScrollRect.Right = CSBufInfo.srWindow.Right;
	ScrollRect.Bottom = CSBufInfo.dwCursorPosition.Y;
	if (CSBufInfo.dwCursorPosition.X > 0) {
		DestOrg.X = CSBufInfo.dwCursorPosition.X - 1;
		DestOrg.Y = CSBufInfo.dwCursorPosition.Y;
		Fill.Char.AsciiChar = ' ';
		//Fill.Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
		Fill.Attributes = CSBufInfo.wAttributes;
		if (!ScrollConsoleScreenBuffer(hOut, &ScrollRect, NULL, DestOrg, &Fill)
			)
		{
			fprintf(stderr,
				"backspace_char_win32(): "
				"ScrollConsoleScreenBuffer() error - %d\n", GetLastError());
			exit(6);
		}
		if (!SetConsoleCursorPosition(hOut, DestOrg)) {
			fprintf(stderr,
				"backspace_char_win32(): "
				"SetConsoleScreenBufferInfo() error - %d\n", GetLastError());
			exit(6);
		}
	}
}

void
delete_char_win32(HANDLE hOut)
{
	if (!GetConsoleScreenBufferInfo(hOut, &CSBufInfo)) {
		fprintf(stderr,
			"delete_char_win32(): GetConsoleScreenBufferInfo() error - %d\n",
			GetLastError());
		exit(6);
	}
	if (CSBufInfo.dwCursorPosition.X < CSBufInfo.dwSize.X - 1) {
		ScrollRect.Left = CSBufInfo.dwCursorPosition.X + 1;
	}
	ScrollRect.Top = CSBufInfo.dwCursorPosition.Y;
	ScrollRect.Right = CSBufInfo.srWindow.Right;
	ScrollRect.Bottom = CSBufInfo.dwCursorPosition.Y;
	DestOrg.X = CSBufInfo.dwCursorPosition.X;
	DestOrg.Y = CSBufInfo.dwCursorPosition.Y;
	Fill.Char.AsciiChar = ' ';
	//Fill.Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	Fill.Attributes = CSBufInfo.wAttributes;
	if (!ScrollConsoleScreenBuffer(hOut, &ScrollRect, NULL, DestOrg, &Fill)) {
		fprintf(stderr,
			"delete_char_win32(): ScrollConsoleScreenBuffer() error - %d\n",
			GetLastError());
		exit(6);
	}
}

