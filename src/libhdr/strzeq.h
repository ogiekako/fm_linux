/*
 * strzeq.h -- extra string manipulate routines
 * Copyright(c) KAMINA, Jiro 1997.  All rights reserved.
 * $Header: strzeq.hv  1.2  03/11/27 16:17:01  Exp $
*/

#if !defined(STRZEQ_H_)
#define	STRZEQ_H_

#define	ZEQ_NOTEQ	-1
#define	ZEQ_EQUAL	0
#define	ZEQ_EXCHG	1
#define	ZEQ_1CHAR	2
#define	ZEQ_1LESS	3
#define	ZEQ_1MORE	4

#if !defined(EQUAL)
#undef	EQUAL
#endif
#define	EQUAL(x,y)	(strcmp((x), (y)) == 0)

typedef	int boolean;

#endif  /* !STRZEQ_H_ */
