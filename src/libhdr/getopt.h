// getopt.h - optional arguments analyzing routines header
// Copyright(c) ISHIGURO, Shuntaro 1997-2007.  All rights reserved.
// $Header: getopt.hv  1.1  07/07/29 20:57:54  Exp $

#if !defined(GETOPT_H_)
#define	GETOPT_H_

// optvar.c
extern char *optarg;  // initial value = NULL
extern int optind;    // initial value = 1

// getopt.c
extern int getopt(int argc, char *argv[], char *pattern);

#endif  // !GETOPT_H_

