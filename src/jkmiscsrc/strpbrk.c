/*
 * strpbrk.c -- SHIFT-JIS string mamipulator
 * Copyright(c) Jiro Kamina 1994-1997.  All rights reserved.
 * $Header: strpbrk.cv  1.1  98/09/22 12:04:08  Exp $
*/

#include <stdio.h>
#include <string.h>

#include "sjis.h"
#include "stringjk.h"

/* strpbrk() for SHIFT-JIS string
*/
char *
strpbrk_sj(const char *s, const char *t)
{
	const char *p;

	for (p = s; *p != '\0'; p++) {
		if (ISKANJI((unsigned char) *p)) {
			p++;
			if (*p == '\0') {
				break;
			}
		} else if (strchr_sj(t, *p) != NULL) {
			return (char *) p;
		}
	}
	return NULL;
}

