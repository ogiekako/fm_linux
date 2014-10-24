/*
 * pushcp32.c -- push/pop cursor position
 * Copyright(c) KAMINA, Jiro 1997, 1998.  All rights reserved.
 * $Header: pushcp32.cv  1.2  03/11/27 12:25:25  Exp $
*/

#include <stdio.h>
#include <windows.h>
#include <wincon.h>

#include "cons32.h"

static char copyright[] =
	"push_cursor_position_win32(), pop_cursor_position_win32(), : "
	"COPYRIGHT(c) KAMINA, Jiro 1997, 1998 "
	"- $Header: pushcp32.cv  1.2  03/11/27 12:25:25  Exp $"
;

void
push_cursor_position_win32(HANDLE hOut)
{
	struct curpos_stack *cpsp;

	cpsp = (struct curpos_stack *)malloc(sizeof(*curpos_stack_top));
	if (cpsp == NULL) {
		fprintf(stderr, "push_cursor_position_win32(): malloc() error - %d\n",
			errno);
		exit(1);
	}
	cpsp->next = curpos_stack_top;
	curpos_stack_top = cpsp;
	get_cursor_position_win32(hOut, &cpsp->x, &cpsp->y);
}

void
pop_cursor_position_win32(HANDLE hOut)
{
	struct curpos_stack *cpsp;

	cpsp = curpos_stack_top;
	if (cpsp == NULL) {
		return;
	}
	locate_win32(hOut, cpsp->x, cpsp->y);
	curpos_stack_top = cpsp->next;  /* == curpos_stack_top->next */
	free(cpsp);
}

