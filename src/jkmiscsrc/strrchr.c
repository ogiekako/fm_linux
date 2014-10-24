/*
 * strrchr.c -- SHIFT-JIS string mamipulator
 * Copyright(c) Jiro Kamina 1993-1997.  All rights reserved.
 * $Header: strrchr.cv  1.1  98/09/22 12:04:08  Exp $
*/

#include <stdio.h>
#include <string.h>

#include "sjis.h"
#include "stringjk.h"

/* strrchr() for SHIFT-JIS string
*/
char *
strrchr_sj(const char *s, int c)
{
	const char *p;

	p = strrchr(s, c);
	if (p == NULL || c < 0x3f) {
		return (char *) p;
	}
	for (;;) {
		if (nthctype(s, p - s) != CT_KJ2) {
			return (char *) p;
		}
		while (*--p != c) {
			if (p <= s) {
				return NULL;
			}
		}
	}
}

