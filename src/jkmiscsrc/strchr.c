/*
 * strchr.c -- SHIFT-JIS string mamipulator
 * Copyright(c) Jiro Kamina 1993-1997.  All rights reserved.
 * $Header: strchr.cv  1.1  98/09/22 12:04:08  Exp $
*/

#include <stdio.h>
#include <string.h>

#include "sjis.h"
#include "stringjk.h"

/* strchr() for SHIFT-JIS string
*/
char *
strchr_sj(const char *s, int c)
{
	const char *p;

	for (p = strchr(s, c); p != NULL; p = strchr(p + 1, c)) {
		if (nthctype(s, p - s) != CT_KJ2) {
			return (char *) p;
		}
	}
	return NULL;
}

