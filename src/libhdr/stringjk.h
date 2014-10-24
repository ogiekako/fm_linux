/* 
 * stringjk.h -- string manipulate routines in shifted-JIS
 * Copyright(c) Jiro Kamina 1997.  All rights reserved.
 * $Header: stringjk.hv  1.1  98/09/22 12:04:18  Exp $
*/

#if !defined(STRINGJK_H_)
#define	STRINGJK_H_

/* strchr.c */
extern char *strchr_sj(const char *s, int c);

/* strrchr.c */
extern char *strrchr_sj(const char *s, int c);

/* strstr.c */
extern char *strstr_sj(const char *s, const char *t);

/* strpbrk.c */
extern char *strpbrk_sj(const char *s, const char *t);

#endif  /* !STRINGJK_H_ */

