// nfm/co/console.c -- console control functions
// Copyleft(c) KAMINA Jiro 2001-2010.  All lefts reserved.
// $Header$

#include	<stdio.h>
#include	<windows.h>
#include	<wincon.h>

static HANDLE hConsOut, hConsIn;
static CHAR chSpaces[2048];
static CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
static COORD dwCurPos;
static DWORD fdwOldConsInMode;

void
win32api_console_initialize(void)
{
	DWORD fdwMode;

	hConsIn = GetStdHandle(STD_INPUT_HANDLE);
	if (hConsIn == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "win32api_console_initialize(): "
			"GetStdHandle() error - %d\n", GetLastError());
		exit(6);
	}
	if (!GetConsoleMode(hConsIn, &fdwOldConsInMode)) {
		fprintf(stderr, "win32api_console_initialize(): "
			"GetConsoleMode() error - %d\n", GetLastError());
		exit(6);
	}
	fdwMode = fdwOldConsInMode;
	fdwMode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT);
	if (!SetConsoleMode(hConsIn, fdwMode)) {
		fprintf(stderr, "win32api_console_initialize(): "
			"SetConsoleMode() error - %d\n", GetLastError());
		exit(6);
	}
	hConsOut = GetStdHandle(STD_ERROR_HANDLE);
	if (hConsOut == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "win32api_console_initialize(): "
			"GetStdHandle() error - %d\n", GetLastError());
		exit(6);
	}
	memset(chSpaces, ' ', sizeof(chSpaces));
}

void
win32api_console_carriage_return(void)
{
	if (!GetConsoleScreenBufferInfo(hConsOut, &csbiInfo)) {
		fprintf(stderr, "win32api_console_carriage_return(): "
			"GetConsoleScreenBufferInfo() error - %d\n", GetLastError());
		exit(6);
	}
	dwCurPos.X = 0;
	dwCurPos.Y = csbiInfo.dwCursorPosition.Y;
	if (!SetConsoleCursorPosition(hConsOut, dwCurPos)) {
		fprintf(stderr, "win32api_console_carriage_return(): "
			"SetConsoleScreenBufferInfo() error - %d\n", GetLastError());
		exit(6);
	}
}

void
win32api_console_clear_line(void)
{
	DWORD dwWritten;

	if (!GetConsoleScreenBufferInfo(hConsOut, &csbiInfo)) {
		fprintf(stderr, "win32api_console_clear_line(): "
			"GetConsoleScreenBufferInfo() error - %d\n", GetLastError());
		exit(6);
	}
	dwCurPos.X = 0;
	dwCurPos.Y = csbiInfo.dwCursorPosition.Y;
	dwWritten = 0;
	if (!FillConsoleOutputAttribute(hConsOut, csbiInfo.wAttributes, 80, dwCurPos, &dwWritten))
	{
		fprintf(stderr, "win32api_console_clear_line(): "
			"FillConsoleOutputAttribute() error - %d\n", GetLastError());
		exit(6);
	}
	dwWritten = 0;
	if (!FillConsoleOutputCharacter(hConsOut, ' ', 80, dwCurPos, &dwWritten)) {
		fprintf(stderr, "win32api_console_clear_line(): "
			"FillConsoleOutputCharacter() error - %d\n", GetLastError());
		exit(6);
	}
	dwCurPos.X = 0;
	dwCurPos.Y = csbiInfo.dwCursorPosition.Y;
	if (!SetConsoleCursorPosition(hConsOut, dwCurPos)) {
		fprintf(stderr, "win32api_console_clear_line(): "
			"SetConsoleScreenBufferInfo() error - %d\n", GetLastError());
		exit(6);
	}
}

void
win32api_console_cursor_up(int y)
{
	if (!GetConsoleScreenBufferInfo(hConsOut, &csbiInfo)) {
		fprintf(stderr, "win32api_console_cursor_up(): "
			"GetConsoleScreenBufferInfo() error - %d\n", GetLastError());
		exit(6);
	}
	dwCurPos.X = csbiInfo.dwCursorPosition.X;
	dwCurPos.Y = csbiInfo.dwCursorPosition.Y - y;
	if (dwCurPos.Y < 0) {
		dwCurPos.Y = 0;
	}
	if (!SetConsoleCursorPosition(hConsOut, dwCurPos)) {
		fprintf(stderr, "win32api_console_cursor_up(): "
			"SetConsoleCursorPosition() error - %d\n", GetLastError());
		exit(6);
	}
}

#if 0
void
win32api_console_cursor_down(int y)
{
	if (!GetConsoleScreenBufferInfo(hConsOut, &csbiInfo)) {
		fprintf(stderr, "win32api_console_cursor_down(): "
			"GetConsoleScreenBufferInfo() error - %d\n", GetLastError());
		exit(6);
	}
	dwCurPos.X = csbiInfo.dwCursorPosition.X;
	dwCurPos.Y = csbiInfo.dwCursorPosition.Y + y;
	if (dwCurPos.Y >= csbiInfo.dwSize.Y) {
		dwCurPos.Y = csbiInfo.dwSize.Y - 1;
	}
	if (!SetConsoleCursorPosition(hConsOut, dwCurPos)) {
		fprintf(stderr, "win32api_console_cursor_down(): "
			SetConsoleCursorPosition() error - %d\n", GetLastError());
		exit(6);
	}
}
#endif

void
win32api_console_reset(void)
{
	if (!SetConsoleMode(hConsIn, fdwOldConsInMode)) {
		fprintf(stderr, "win32api_console_reset(): "
			"SetConsoleMode() error - %d\n", GetLastError());
		exit(6);
	}
	CloseHandle(hConsIn);
	CloseHandle(hConsOut);
}

