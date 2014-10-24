/*
 * console.h -- console I/O
 * Copyright(c) KAMINA, Jiro 1997-2003.  All rights reserved.
 * $Header: console.hv  1.2  03/11/27 16:10:26  Exp $
*/

#if !defined(CONSOLE_H_)
#define	CONSOLE_H_

#include "cons32.h"
#define INIT_CONS()			init_console_win32()
#define RESET_CONS()		reset_console_win32()
#define GETCH()				get_console_char_win32(hStdin)
#define	PUT_CONS(t)			put_console_win32(hStdout, (t))
#define	PUTCH(c)			put_console_char_win32(hStdout, (c))
#define	SET_COLOR(f, b)		set_text_attrib_win32(hStdout, (f) | (b))
#define	FOREGROUND_WHITE	(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED)
#define	FOREGROUND_WHITEX	(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED \
							| FOREGROUND_INTENSITY)
#define	FOREGROUND_BLUEX	(FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define	FOREGROUND_GREENX	(FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define	FOREGROUND_REDX		(FOREGROUND_RED | FOREGROUND_INTENSITY)
#define	BACKGROUND_WHITE	(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED)
#define	BACKGROUND_WHITEX	(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED \
							| BACKGROUND_INTENSITY)
#define	BACKGROUND_BLUEX	(BACKGROUND_BLUE | BACKGROUND_INTENSITY)
#define	BACKGROUND_GREENX	(BACKGROUND_GREEN | BACKGROUND_INTENSITY)
#define	BACKGROUND_REDX		(BACKGROUND_RED | BACKGROUND_INTENSITY)

#define ERASE_CSB()			erase_csb_win32(hStdout)
#define CURSOR_FORWARD_WRAPPED(x)	cursor_forward_wrapped_win32(hStdout, (x))
#define CURSOR_BACK_WRAPPED(x)		cursor_back_wrapped_win32(hStdout, (x))
#define CURSOR_BACK_WRAPPED_1()		cursor_back_wrapped_win32(hStdout, 1)
#define ERASE_EOF_WRAPPED(l)		erase_eof_wrapped_win32(hStdout, (l))
#define BACKSPACE()			backspace_char_win32(hStdout)
#define DEL_CHAR()			delete_char_win32(hStdout)

#endif  /* !CONSOLE_H_ */

