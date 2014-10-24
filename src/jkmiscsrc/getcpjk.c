/*
 * getcpjk.c -- get cursor position
 * Copyright(c) Jiro Kamina 1995-1998.  All rights reserved.
 * $Header: getcpjk.cv  1.1  98/11/21 12:26:22  Exp $
*/

#include <stdio.h>
#if defined(__TURBOC__) || defined(_MSC_VER)
#include <conio.h>
#elif defined(__CYGWIN32__)
#define	getch()		_getch()
#define	kbhit()		_kbhit()
#elif defined(__GNUC__)
#include "consjk.h"
#define	getch()		getch_pc98()
#define	kbhit()		kbhit_pc98()
#endif

/*
 * PC-98	"\033[%02d;%02dR"
 * DOS/V	"\033[%02d;%02dR\n"
 * FMR		"\033[%d;%dR\n"
*/
void
get_cursor_position_dos(FILE *out, int *y, int *x)
{
	int c;

	fputs("\033[6n", out);
	(void) getch();  /* \033 */
	(void) getch();  /* [ */
	*y = getch() - '0';
	c = getch();
	if (c != ';') {  /* for FMR */
		*y = *y * 10 + c - '0';
		(void) getch();  /* ; */
	}
	*x = getch() - '0';
	c = getch();
	if (c != 'R') {  /* for FMR */
		*x = *x * 10 + c - '0';
		(void) getch();  /* R */
	}
	if (kbhit() != 0) {
		(void) getch();  /* \n (DOS/V, FMR) */
	}
}

