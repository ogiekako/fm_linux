/*
 * dosopt.h - DOS optional arguments analyzing routines header
 * Copyright(c) KAMINA, Jiro 1997.  All rights reserved.
 * $Header: dosopt.hv  1.2  03/11/27 16:13:37  Exp $
*/

#if !defined(DOSOPT_H_)
#define	DOSOPT_H_

#include "sjis.h"

/* optvar.c */
extern char *optarg;  /* initial value = NULL */
extern int optind;    /* initial value = 1 */

#if defined(__cplusplus)
extern "C" {
#endif

/* dosopt.c */
extern int dosopt(char *argv[], const char *pattern);

#if defined(__cplusplus)
}
#endif

#endif  /* !DOSOPT_H_ */

