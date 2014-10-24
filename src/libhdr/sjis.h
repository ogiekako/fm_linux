/*
 * sjis.h -- sifted-JIS character manipulate routines and macros
 * Copyright(c) KAMINA, Jiro 1992-1997.  All rights reserved.
 * $Header: sjis.hv  1.2  03/11/27 16:15:43  Exp $
*/

#if !defined(SJIS_H_)
#define	SJIS_H_

#if !defined(CT_ANK)
#define	CT_ANK		0
#endif
#if !defined(CT_KJ1)
#define	CT_KJ1		1
#endif
#if !defined(CT_KJ2)
#define	CT_KJ2		2
#endif

#if  defined(__GNUC__) || defined(_MSC_VER) || defined(__BORLANDC__)
#define	ISKANJI(c)		((0x81 <= (c) && (c) <= 0x9f) || (0xe0 <= (c) && (c) <= 0xfc))
#define	ISKANA(c)		((c) >= 0xa1 && (c) <= 0xdf)
#define	JISDIGIT(w)		((w) >= 0x824f && (w) <= 0x8258)
#define	JISUPPER(w)		((w) >= 0x8260 && (w) <= 0x8279)
#define	JISLOWER(w)		((w) >= 0x8281 && (w) <= 0x829a)
#elif defined(__TURBOC__)
#define	ISKANJI(c)		iskanji(c)
#define	ISKANA(c)		iskana(c)
#define	JISDIGIT(w)		jisdigit(w)
#define	JISUPPER(w)		jisupper(w)
#define	JISLOWER(w)		jislower(w)
#endif

/* nthctype.c */
extern int nthctype(const char *s, int n);

/* jis2sj.c */
extern unsigned short jis2sj(unsigned short us);

#endif  /* !SJIS_H_ */

