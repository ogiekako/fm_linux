/*
 * dosopt.c -- DOS style optional argument analyzer
 *             optional argument and default value of argument supported
 * Copyright (c) Jiro Kamina 1992-1997.  All rights reserved.
 * $Header: dosopt.cv  1.1  02/12/01 17:37:33  Exp $
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mbstring.h>
#include <ctype.h>
#include <mbctype.h>

#include "getoptjk.h"

#define	OFF		(0)
#define	ON		(-1)

static char *
strupr(char *s)
{
	char *t;

	for (t = s; t[0] != '\0'; t++) {
		if (_ismbblead((unsigned char)t[0])) {
			t++;
		} else if (islower(t[0])) {
			t[0] -= 'a' - 'A';
		}
	}
	return s;
}

int
dosopt(char **argv, const char *pattern)
{
	const char *s, *t;
	static int eof = OFF;
	static char *arg = NULL;
	static char *next_arg = NULL;

	optarg = NULL;

	if (next_arg != NULL) {
		next_arg[0] = '/';
		next_arg = NULL;
	}

	if (argv == NULL || eof == ON) {
		return EOF;
	}

	if (pattern == NULL) {
		return EOF;
	}

	if (arg == NULL) {
		if (argv[optind] == NULL) {
			eof = ON;
			return EOF;
		}
		arg = strupr(argv[optind]);
#if !defined(MIXED_KEY_PARM)
		if (arg == NULL || arg[0] != '/') {
			eof = ON;
			return EOF;
		}
#endif
	}
	next_arg = strchr(arg + 1, '/');
	if (next_arg != NULL) {
		next_arg[0] = '\0';
	} else {
		optind++;
	}
#if defined(MIXED_KEY_PARM)
	if (arg[0] != '/') {
		optarg = arg;
		arg = next_arg;
		return '*';
	}
#endif

	for (s = pattern; s[0] != '\0'; s++) {
		if (s[0] == arg[1]) {
			break;
		}
		if (s[0] == '[') {
			t = _mbschr(s, ']');
			if (t == NULL) {
				s = strchr(s, '\0') - 1;
			} else {
				s = t;
			}
		}
	}
	if (s[0] == '\0') {
		arg = NULL;
		return '?';
	}

	if (s[1] == ':') {
		if (arg[2] == '\0') {
			arg = NULL;
			return '?';
		}
		optarg = arg + 2;
	} else if (s[1] == '[') {
		if (arg[2] == '\0' || arg[2] == '/') {
			t = _mbschr(s + 2, ']');
			if (t == NULL) {
				exit(2);
			}
			optarg = malloc((t - s) - 1);
			if (optarg == NULL) {
				exit(2);
			}
			optarg[0] = '\0';
			(void)strncat(optarg, s + 2, (t - s) - 2);
		} else {
			optarg = arg + 2;
		}
	} else {
		if (!(arg[2] == '\0' || arg[2] == '/')) {
			arg = NULL;
			return '?';
		}
	}
	arg = next_arg;
	return s[0];
}

