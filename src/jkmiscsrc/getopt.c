/*
 * getopt.c -- optional argument analizer
 * Copyright(c) Jiro Kamina 1991-1997.  All rights reserved.
 * $Header: getopt.cv  1.1  98/09/22 12:04:08  Exp $
*/

#include <stdio.h>
#include <string.h>

#include "getoptjk.h"

#define	OFF		0
#define	ON		1

int
getopt(int argc, char *argv[], char *pattern)
{
	char *s;
	static char *arg;
	static int rest = OFF;
	static int eof = OFF;

	if (eof == ON) {
		return EOF;
	}

	if (rest == ON) {
		if ((s = strchr(pattern, arg[0])) == NULL || *(s + 1) == ':') {
			s = "?";
		}
		if (arg[1] == '\0') {
			rest = OFF;
			optind++;
		} else {
			arg++;
		}
		return *s;
	}

	arg = argv[optind];
	optarg = NULL;
	if (arg == NULL || arg[0] != '-') {
		eof = ON;
		return EOF;
	}

	/*
	 * arg[0] == '-'
	 */
	if (arg[1] == '-' && arg[2] == '\0') {
		/*
		 * arg == "--"
		 */
		optind++;
		return EOF;
	}
	if ((s = strchr(pattern, arg[1])) == NULL) {
		if (arg[2] == '\0') {
			optind++;
		} else {
			rest = ON;
			arg += 2;
		}
		return '?';
	}
	if (*(s + 1) == ':') {
		if (arg[2] == '\0') {
			optind++;
			if (argv[optind]) {
				optarg = argv[optind];
				optind++;
			} else {
				eof = ON;
				return '?';
			}
		} else {
			optarg = arg + 2;
			while (*optarg == ' ') {
				optarg++;
			}
			optind++;
		}
	} else {
		if (arg[2] == '\0') {
			optind++;
		} else {
			rest = ON;
			arg += 2;
		}
	}
	return *s;
}

