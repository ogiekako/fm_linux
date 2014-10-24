/*
 * cons32.h -- Win32 API console sub-routine header
 * Copyright(c) KAMINA, Jiro 1997-2003.  All rights reserved.
 * $Header: cons32.hv  1.2  03/11/27 16:13:13  Exp $
*/

#if !defined(CONS32_H_)
#define	CONS32_H_

#include <windows.h>
#include <wincon.h>
#include <conio.h>

/* these are not Win32 API but relative */
#define	getch()			_getch()
#define	kbhit()			_kbhit()
#define	ungetch(c)		_ungetch(c)

struct curpos_stack {
	struct curpos_stack *next;
	int x;
	int y;
};

#if defined(__cplusplus)
extern "C" {
#endif

/* inicon32.c */
extern struct curpos_stack *curpos_stack_top;
extern HANDLE hStdin, hStdout, hStderr;
extern void init_console_win32(void);
extern void reset_console_win32(void);

/* bschar32.c */
extern void backspace_char_win32(HANDLE hOut);
extern void delete_char_win32(HANDLE hOut);

/* clrln32.c */
extern void clear_line_win32(HANDLE hOut);
extern void clear_line_wrapped_win32(HANDLE hOut, int lines);

/* clrscr32.c */
extern void clear_screen_win32(HANDLE hOut);

/* cr32.c */
extern void carriage_return_win32(HANDLE hOut);

/* curbk32.c */
extern void cursor_back_win32(HANDLE hOut, int x);
extern void cursor_forward_win32(HANDLE hOut, int x);
extern void cursor_back_wrapped_win32(HANDLE hOut, int x);
extern void cursor_forward_wrapped_win32(HANDLE hOut, int x);

/* curon32.c */
extern void cursor_on_win32(HANDLE hOut);
extern void cursor_off_win32(HANDLE hOut);

/* curup32.c */
extern void cursor_up_win32(HANDLE hOut, int y);
extern void cursor_down_win32(HANDLE hOut, int y);

/* erscsb32.c */
extern void erase_csb_win32(HANDLE hOut);

/* erseof32.c */
extern void erase_eof_win32(HANDLE hOut);
extern void erase_eof_wrapped_win32(HANDLE hOut, int lines);

/* erseos32.c */
extern void erase_eos_win32(HANDLE hOut);

/* getcls32.c */
extern int get_console_lines_win32(void);

/* getcon32.c */
extern int get_console_win32(HANDLE hIn, LPVOID buf, DWORD rc);
extern int get_console_char_win32(HANDLE hIn);

/* getcp32.c */
extern void get_cursor_position_win32(HANDLE hIn, int *y, int *x);

/* getnor32.c */
extern int get_nobufrows_win32(HANDLE hOut);
extern int get_nodisprows_win32(HANDLE hOut);

/* inscon32.c */
extern int insert_console_win32(HANDLE hOut, char *s);

/* locate32.c */
extern void locate_win32(HANDLE hOut, int y, int x);

/* pushcp32.c */
extern void push_cursor_position_win32(HANDLE hOut);
extern void pop_cursor_position_win32(HANDLE hOut);

/* putcon32.c */
extern int put_console_win32(HANDLE hOut, const char *s);
extern int put_console_char_win32(HANDLE hOut, int c);

/* settxa32.c */
void set_text_attrib_win32(HANDLE hOut, WORD attrib);
WORD get_text_attrib_win32(HANDLE hOut);

#if defined(__cplusplus)
}
#endif

#endif  /* !CONS32_H_ */

