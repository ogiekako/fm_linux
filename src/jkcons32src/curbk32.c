/*
 * curbk32.c -- cursor back and forward
 * Copyright(c) KAMINA, Jiro 1997-2003.  All rights reserved.
 * $Header: curbk32.cv  1.2  03/11/27 12:31:03  Exp $
*/

#include <stdio.h>
#include <windows.h>
#include <wincon.h>

#include "cons32.h"

static char copyright[] =
	"cursor_back_win32(), cursor_forward_win32(), "
	"cursor_back_wrapped_win32(), cursor_forward_wrapped_win32(): "
	"COPYRIGHT(c) KAMINA, Jiro 1997-2003 "
	"- $Header: curbk32.cv  1.2  03/11/27 12:31:03  Exp $"
;

static CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
static COORD dwCurPos;

void
cursor_back_win32(HANDLE hOut, int x)
{
	if (!GetConsoleScreenBufferInfo(hOut, &csbiInfo)) {
		fprintf(stderr,
			"cursor_back_win32(): GetConsoleScreenBufferInfo() error - %d\n",
			GetLastError());
		exit(6);
	}
	dwCurPos.X = csbiInfo.dwCursorPosition.X - x;
	dwCurPos.Y = csbiInfo.dwCursorPosition.Y;
	if (dwCurPos.X < 0) {
		dwCurPos.X = 0;
	}
	if (!SetConsoleCursorPosition(hOut, dwCurPos)) {
		fprintf(stderr,
			"cursor_back_win32(): SetConsoleCursorPosition() error - %d\n",
			GetLastError());
		exit(6);
	}
}

void
cursor_forward_win32(HANDLE hOut, int x)
{
	if (!GetConsoleScreenBufferInfo(hOut, &csbiInfo)) {
		fprintf(stderr,
			"cursor_forward_win32(): GetConsoleScreenBufferInfo() error - %d\n",
			GetLastError());
		exit(6);
	}
	dwCurPos.X = csbiInfo.dwCursorPosition.X + x;
	dwCurPos.Y = csbiInfo.dwCursorPosition.Y;
	if (dwCurPos.X >= csbiInfo.dwSize.X - 1) {
		dwCurPos.X = csbiInfo.dwSize.X - 1 - 1;
	}
	if (!SetConsoleCursorPosition(hOut, dwCurPos)) {
		fprintf(stderr,
			"cursor_forward_win32(): SetConsoleCursorPosition() error - %d\n",
			GetLastError());
		exit(6);
	}
}

void
cursor_back_wrapped_win32(HANDLE hOut, int x)
{
	if (!GetConsoleScreenBufferInfo(hOut, &csbiInfo)) {
		fprintf(stderr,
			"cursor_back_wrapped_win32(): "
			"GetConsoleScreenBufferInfo() error - %d\n", GetLastError());
		exit(6);
	}
	dwCurPos.X = csbiInfo.dwCursorPosition.X - x;
	dwCurPos.Y = csbiInfo.dwCursorPosition.Y;
	while (dwCurPos.X < 0) {
		if (dwCurPos.Y == 0) {
			dwCurPos.X = 0;
			break;
		}
		dwCurPos.Y--;
		dwCurPos.X += csbiInfo.dwSize.X;
	}
	if (!SetConsoleCursorPosition(hOut, dwCurPos)) {
		fprintf(stderr,
			"cursor_back_wrapped_win32(): "
			"SetConsoleCursorPosition() error - %d\n", GetLastError());
		//fprintf(stderr, "cursor_forward_wrapped_win32(): "
		//	"cursor_position: X=%d, Y=%d\n", dwCurPos.X, dwCurPos.Y);
		exit(6);
	}
}

void
cursor_forward_wrapped_win32(HANDLE hOut, int x)
{
	if (!GetConsoleScreenBufferInfo(hOut, &csbiInfo)) {
		fprintf(stderr,
			"cursor_forward_wrapped_win32(): "
			"GetConsoleScreenBufferInfo() error - %d\n", GetLastError());
		exit(6);
	}
	dwCurPos.X = csbiInfo.dwCursorPosition.X + x;
	dwCurPos.Y = csbiInfo.dwCursorPosition.Y;
	while (dwCurPos.X >= csbiInfo.dwSize.X) {
		if (dwCurPos.Y >= csbiInfo.dwSize.Y) {
			dwCurPos.X = csbiInfo.dwSize.X - 1 - 1;
			break;
		}
		dwCurPos.Y++;
		dwCurPos.X -= csbiInfo.dwSize.X;
	}
	if (!SetConsoleCursorPosition(hOut, dwCurPos)) {
		fprintf(stderr,
			"cursor_forward_wrapped_win32(): "
			"SetConsoleCursorPosition() error - %d\n", GetLastError());
		//fprintf(stderr, "cursor_forward_wrapped_win32(): "
		//	"cursor_position: X=%d, Y=%d\n", dwCurPos.X, dwCurPos.Y);
		exit(6);
	}
}

