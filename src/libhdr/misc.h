/*
 * misc.h -- miscellaneous routines and macros
 * Copyright(c) KAMINA, Jiro 1997-2000.  All rights reserved.
 * $Header: misc.hv  1.2  03/11/27 16:16:18  Exp $
*/

#if !defined(MISC_H_)
#define	MISC_H_

#include <string.h>

#define	ARRAY_SIZE_OF(a)	(sizeof(a)/sizeof(*(a)))
#define	STR_SIZE_OF(s)		(sizeof(s) - 1)
#define	EQUAL(a, b)			(strcmp((a), (b)) == 0)
#define	EQUALN(a, b, n)		(strncmp((a), (b), (n)) == 0)
#define	EQUALM(a, b, n)		(memcmp((a), (b), (n)) == 0)
#define	FOREVER				for ( ; ; )
#define	SKIP_SPACE(s)		((s) + strspn((s), " \t"))
#define	SKIP_CHAR(s)		((s) + strcspn((s), " \t"))
#define	BEEP()				putc('\007', stdout)

#if defined(_MSC_VER) || defined(__BORLANDC__)
#include "misc32.h"
#define	DELAY(t)			delay_win32(t)
/* PC-98 series only */
/* #elif defined(__TURBOC__) */
/* #include "dos.h" */
/* #define	DELAY(t)			delay(t) */
#else
#include "miscjk.h"
#define	DELAY(t)			delay_jk(t)
#endif

#if defined(__GNUC__)
/* itoajk.c */
extern char *itoa(int n, char *s, int r);
#endif

#if defined(__GNUC__)
/* maxjk.c */
extern int max(int a, int b);
extern int min(int a, int b);
#endif

#endif  /* !MISC_H_ */

