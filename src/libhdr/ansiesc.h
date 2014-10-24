/*
 * ansiesc.h -- ANSI escape sequence macros
 * Copyright(c) KAMINA, Jiro 1997-2003.  All rights reserved.
 * $Header: ansiesc.hv  1.2  03/11/27 16:08:34  Exp $
*/

#if !defined(ANSIESC_H_)
#define	ANSIESC_H_

#include "cons32.h"

#define CLEAR_LINE()		clear_line_win32(hStdout)
#define CLEAR_SCREEN()		clear_screen_win32(hStdout)
#define ERASE_EOF()			erase_eof_win32(hStdout)
#define LOCATE(y, x)		locate_win32(hStdout, (y), (x))
#define CURSOR_FORWARD(x)	cursor_forward_win32(hStdout, (x))
#define CURSOR_BACK(x)		cursor_back_win32(hStdout, (x))
#define CURSOR_BACK_1()		cursor_back_win32(hStdout, 1)
#define CARRIAGE_RETURN()	carriage_return_win32(hStdout)
#define CURSOR_UP(y)		cursor_up_win32(hStdout, (y))
#define CURSOR_DOWN(y)		cursor_down_win32(hStdout, (y))
#define	PUSH_CURPOS()		push_cursor_position_win32(hStdout)
#define	POP_CURPOS()		pop_cursor_position_win32(hStdout)
#define	GET_CURPOS(y, x)	get_cursor_position_win32(hStdout, (y), (x))

#define ERASE_EOS(x)		erase_eos_win32(hStdout)
#define CURSOR_ON(x, y)		cursor_on_win32(hStdout)
#define CURSOR_OFF(x, y)	cursor_off_win32(hStdout)

#endif  /* !ANSIESC_H_ */

