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

	/* �ڕW�ʒu t �ɒB����܂ŁA�����̃o�C�g���ɉ����� s ��i�߂� */
	while (s < t) {
		if (ISKANJI((unsigned char) *s)) {
			s += 2;
		} else {
			s++;
		}
	}

	if (s == t) {  /* �҂�����̈ʒu��������A���߂āA���ʂɔ��肷�� */
		if (ISKANJI((unsigned char) *s)) {
			return CT_KJ1;
		} else {
			return CT_ANK;
		}
	} else {  /* ��щz���Ă�����A�����R�[�h�̂Q�o�C�g�ڂƂ��� */
		return CT_KJ2;
	}
}

