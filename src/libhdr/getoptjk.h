/* 
 * getoptjk.h - optional arguments analyzing routines header
 * Copyright(c) Jiro Kamina 1997.  All rights reserved.
 * $Header: getoptjk.hv  1.1  98/09/22 12:04:18  Exp $
*/

#if !defined(GETOPTJK_H_)
#define	GETOPTJK_H_

#include "sjis.h"

/* optvar.c */
extern char *optarg;  /* initial value = NULL */
extern int optind;    /* initial value = 1 */

/* getopt.c */
extern int getopt(int argc, char *argv[], char *pattern);

#endif  /* !GETOPTJK_H_ */

