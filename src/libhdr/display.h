/*
 * display.h -- display macros
 * Copyright(c) KAMINA, Jiro 1997.  All rights reserved.
 * $Header: display.hv  1.2  03/11/27 16:12:53  Exp $
*/

#if !defined(DISPLAY_H_)
#define	DISPLAY_H_

#include <stdio.h>

#define	DISPLAYC(c)			fputc((c), stdout)
#define	DISPLAY(t)			fputs((t), stdout)
#define	DISPLAY0(t)			fputs((t), stdout)
#define	DISPLAY1(t, a)		printf((t), (a))
#define	DISPLAY2(t, a, b)	printf((t), (a), (b))
#define	DISPLAY3(t, a, b, c)  printf((t), (a), (b), (c))
#define	DISPLAY4(t, a, b, c, d)  printf((t), (a), (b), (c), (d))
#define	DISPLAY5(t, a, b, c, d, e)  printf((t), (a), (b), (c), (d), (e))
#define	DISPLAY6(t, a, b, c, d, e, f) \
	printf((t), (a), (b), (c), (d), (e), (f))
#define	DISPLAY7(t, a, b, c, d, e, f, g) \
	printf((t), (a), (b), (c), (d), (e), (f), (g))
#define	DISPLAY8(t, a, b, c, d, e, f, g, h) \
	printf((t), (a), (b), (c), (d), (e), (f), (g), (h))
#define	SPACE()				putchar('\n')
#define	SPACE2()			fputs("\n\n", stdout)
#define	NEWLINE()			putchar('\n')
#define	BLANK()				putchar(' ');

#define	DISPERRC(c)			fputc((c), stderr)
#define	DISPERR(t)			fputs((t), stderr)
#define	DISPERR0(t)			fputs((t), stderr)
#define	DISPERR1(t, a)		fprintf(stderr, (t), (a))
#define	DISPERR2(t, a, b)	fprintf(stderr, (t), (a), (b))
#define	DISPERR3(t, a, b, c)  fprintf(stderr, (t), (a), (b), (c))
#define	DISPERR4(t, a, b, c, d)  fprintf(stderr, (t), (a), (b), (c), (d))
#define	DISPERR5(t, a, b, c, d, e) \
	fprintf(stderr, (t), (a), (b), (c), (d), (e))
#define	DISPERR6(t, a, b, c, d, e, f) \
	fprintf(stderr, (t), (a), (b), (c), (d), (e), (f))
#define	DISPERR7(t, a, b, c, d, e, f, g) \
	fprintf(stderr, (t), (a), (b), (c), (d), (e), (f), (g))
#define	DISPERR8(t, a, b, c, d, e, f, g, h) \
	fprintf(stderr, (t), (a), (b), (c), (d), (e), (f), (g), (h))
#define	SPACE_E()			putc('\n', stderr)
#define	SPACE2_E()			fputs("\n\n", stderr)
#define	NEWLINE_E()			putc('\n', stderr)
#define	BLANK_E()			putc(' ', stderr);

#endif  /* !DISPLAY_H_ */

