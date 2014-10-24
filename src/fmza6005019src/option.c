// zaco/option.c -- FM option
// Copyright(c) KAMINA, Jiro 2013-2014. All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/option.c 1.9 2014/08/14 02:50:47 budai10 Exp budai10 $

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<mbstring.h>
#include	"fm.h"
#include	"piece.h"

void
option_L(char *optarg)
{
	char *s, *t;

	// /L0 means only testing initial phase
	s = strtok(optarg, ",");
	if (s == NULL) {
		return;
	}
	do {
		solution_limit = strtol(s, &t, 10);
	} while ((s = strtok(NULL, ",")) != NULL);

	return;
}

void
option_E(char *optarg)
{
	char *argstr, *s;
	int rc = 0;

	argstr = _strdup(optarg);
	if (argstr == NULL) {
		fm_exit_mae("/Eオプション領域");
	}
	s = _mbsstr(argstr, "FP=");
	if (s != NULL) {
		if (option_E_FP(s, 1) == -1) {
			rc = -1;
		}
	}
	s = _mbsstr(argstr, "F1=");
	if (s != NULL) {
		if (option_E_FP(s, 1) == -1) {
			rc = -1;
		}
	}
	s = _mbsstr(argstr, "F2=");
	if (s != NULL) {
		if (option_E_FP(s, 2) == -1) {
			rc = -1;
		}
	}
	s = _mbsstr(argstr, "F3=");
	if (s != NULL) {
		if (option_E_FP(s, 3) == -1) {
			rc = -1;
		}
	}
	s = _mbsstr(argstr, "F4=");
	if (s != NULL) {
		if (option_E_FP(s, 4) == -1) {
			rc = -1;
		}
	}
	s = _mbsstr(argstr, "F5=");
	if (s != NULL) {
		if (option_E_FP(s, 5) == -1) {
			rc = -1;
		}
	}
	s = _mbsstr(argstr, "F6=");
	if (s != NULL) {
		if (option_E_FP(s, 6) == -1) {
			rc = -1;
		}
	}
	s = _mbsstr(argstr, "F7=");
	if (s != NULL) {
		if (option_E_FP(s, 7) == -1) {
			rc = -1;
		}
	}
	s = strstr(argstr, "MHEH=");
	if (s == NULL) {
		s = strstr(argstr, "M9H=");
	}
	if (s != NULL) {  // give header # for /MHE or /M9
		int i;
		memset(s, '_', STR_SIZE_OF("M9H="));
		s += STR_SIZE_OF("M9H=");
		if (s[0] == '=') {
			s++;
		}
		no_hi_eh_head = strtoul(s, NULL, 10);
		for (i = 0; no_hi_eh_head > 0; i++) {
			no_hi_eh_head >>= 1;
		}
		no_hi_eh_head = 1 << i;
		while (isdigit((uchar_t)s[0])) {
			s[0] = '_';
			s++;
		}
	}
	s = strstr(argstr, "MYEH=");
	if (s == NULL) {
		s = strstr(argstr, "M10H=");
	}
	if (s != NULL) {  // give header # for /MYE or /M10
		int i;
		memset(s, '_', STR_SIZE_OF("MYEH="));
		s += STR_SIZE_OF("MYEH=");
		no_yo_eh_head = strtoul(s, NULL, 10);
		for (i = 0; no_yo_eh_head > 0; i++) {
			no_yo_eh_head >>= 1;
		}
		no_yo_eh_head = 1 << i;
		while (isdigit((uchar_t)s[0])) {
			s[0] = '_';
			s++;
		}
	}
	free(argstr);
	if (rc == -1) {
		fm_exit(1);
	}

	return;
}

void
option_M(char *optarg)
{
	char *s, *t;

	s = strtok(optarg, ",");
	if (s == NULL) {
		return;
	}
	do {
		// strategy
		if (EQUAL(s, "-")) {
			strategy &= ~(B_HI_ESCAPE | B_HI_ESCAPE);
			continue;
		}
		if (EQUAL(s, "HE-") || EQUAL(s, "9-")) {
			strategy &= ~B_HI_ESCAPE;
			continue;
		}
		if (EQUAL(s, "YE-") || EQUAL(s, "10-")) {
			strategy &= ~B_YO_ESCAPE;
			continue;
		}
		if (EQUAL(s, "HE") || EQUALZ(s, "HE=") || EQUAL(s, "9") || EQUALZ(s, "9=")) {
			strategy |= B_HI_DENY_SAME | B_HI_ESCAPE;
		} else if (EQUAL(s, "YE") || EQUALZ(s, "YE=") || EQUAL(s, "10") || EQUALZ(s, "10=")) {
			strategy |= B_YO_DENY_SAME | B_YO_ESCAPE;
		} else if (EQUAL(s, "YE2") || EQUALZ(s, "YE2=") || EQUAL(s, "14") || EQUALZ(s, "14=")) {
			strategy |= B_YO_ESCAPE_2;
		}
		// limits
		if (EQUAL(s, "HE") || EQUAL(s, "9")) {
			hi_eh_lim = NOEHDEFAULT;
		} else if (EQUAL(s, "YE") || EQUAL(s, "10")) {
			yo_eh_lim = NOEHDEFAULT;
		} else if (EQUAL(s, "YE2") || EQUAL(s, "14")) {
			yo_eh2_lim = NOEHDEFAULT;
		} else if (EQUALZ(s, "HE=") || EQUALZ(s, "9=")) {
			hi_eh_given = ON;
			hi_eh_lim = (s[1] == '=') ? strtoul(s + 2, &t, 10) : strtoul(s + 3, &t, 10);
			if (EQUAL(t, "MB")) {
				hi_eh_lim *= 0x100000L;
				hi_eh_lim /= sizeof(escape_hash_t);
			}
			if (hi_eh_lim > NOEHLIMIT) {
				hi_eh_lim = NOEHLIMIT;
			} else if (hi_eh_lim <= NOEHDEFAULT) {
				hi_eh_lim = NOEHDEFAULT;
			}
		} else if (EQUALZ(s, "YE=") || EQUALZ(s, "10=")) {
			yo_eh_given = ON;
			yo_eh_lim = strtoul(s + 3, &t, 10);
			if (EQUAL(t, "MB")) {
				yo_eh_lim *= 0x100000L;
				yo_eh_lim /= sizeof(escape_hash_t);
			}
			if (yo_eh_lim > NOEHLIMIT) {
				yo_eh_lim = NOEHLIMIT;
			} else if (yo_eh_lim <= NOEHDEFAULT) {
				yo_eh_lim = NOEHDEFAULT;
			}
		} else if (EQUALZ(s, "YE2=") || EQUALZ(s, "14=")) {
			yo_eh2_given = ON;
			yo_eh2_lim = (s[2] == '=') ? strtoul(s + 3, &t, 10) : strtoul(s + 4, &t, 10);
			if (EQUAL(t, "MB")) {
				yo_eh2_lim *= 0x100000L;
				yo_eh2_lim /= sizeof(escape_hash_t);
			}
			if (yo_eh2_lim > NOEHLIMIT) {
				yo_eh2_lim = NOEHLIMIT;
			} else if (yo_eh2_lim <= NOEHDEFAULT) {
				yo_eh2_lim = NOEHDEFAULT;
			}
		}
		// allow-same
		if (EQUAL(s, "HAS") || EQUAL(s, "-1")) {
			strategy |= B_HI_ALLOW_SAME;
		} else if (EQUAL(s, "YAS") || EQUAL(s, "-2")) {
			strategy |= B_YO_ALLOW_SAME;
		}
	} while ((s = strtok(NULL, ",")) != NULL);

	return;
}

void
option_X(char *optarg)
{
	char *s;
	char work[64];

	arg_x = _strdup(optarg);
	if (arg_x == NULL) {
		fm_exit_mae("/Xオプション領域");
	}
	strncpy(work, arg_x, sizeof(work));
	toasciiuc(work);
	s = _mbschr(work, 'A');
	if (s != NULL) {
		loose = ON;
		s[0] = '_';
	}
	s = _mbschr(work, 'D');
	if (s != NULL) {
		dancing = ON;
		s[0] = '_';
	}
	s = _mbschr(work, 'V');
	if (s != NULL) {
		verbose = ON;
		s[0] = '_';
	}
	s = _mbspbrk(work, "0123456789");
	if (s == NULL) {
		return;
	}
	// ★for test...
	if (NOT(isxdigit((uchar_t)s[0])) || NOT(isxdigit((uchar_t)s[1]))) {
		fprintf(stderr, "%s: /Xオプションの値が誤っています...\n", cmdname);
		fm_exit(1);
	}
	s[2] = '\0';
	debug |= strtol(s, NULL, 16);

	return;
}

