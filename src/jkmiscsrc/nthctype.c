/*
 * nthctype.c -- test nth character
 * Copyright(c) Jiro Kamina 1993-1997.  All rights reserved.
 * $Header: nthctype.cv  1.1  98/09/22 12:04:08  Exp $
*/

#include <stdio.h>
#include <string.h>

#include "sjis.h"

int
nthctype(const char *s, int n)
{
	const char *t = s + n;

	/* 目標位置 t に達するまで、文字のバイト数に応じて s を進める */
	while (s < t) {
		if (ISKANJI((unsigned char) *s)) {
			s += 2;
		} else {
			s++;
		}
	}

	if (s == t) {  /* ぴったりの位置だったら、改めて、普通に判定する */
		if (ISKANJI((unsigned char) *s)) {
			return CT_KJ1;
		} else {
			return CT_ANK;
		}
	} else {  /* 飛び越えていたら、漢字コードの２バイト目とする */
		return CT_KJ2;
	}
}

