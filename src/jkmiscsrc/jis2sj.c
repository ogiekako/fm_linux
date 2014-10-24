/*
 * jis2sj.c -- JIS to SHIFT-JIS converter
 * Copyright(c) Jiro Kamina 1997.  All rights reserved.
 * $Header: jis2sj.cv  1.1  98/09/22 12:04:08  Exp $
*/

unsigned short
jis2sj(unsigned short us)
{
	unsigned short jh, jl;

	jh = (us >> 8) & 0xff;
	jl = (us & 0xff) + 0x1f;
	if (!(jh & 0x01)) {
		jl += 0x7f - 0x21;
	}
	if (jl >= 0x7f) {
		jl++;
	}
	return (((((jh - 0x21) / 2) + 0xa1) ^ 0x20) << 8) + jl;
}

