// nfm/nfm/macro.h -- macros
// Copyright(c) KAMINA Jiro 2006-2010.  All rights reserved.
// $Header$

#if !defined(MACRO_H_)
#define MACRO_H_

//#define	ARRAY_SIZE_OF(a)	(sizeof(a)/sizeof(*a))
#define	STR_SIZE_OF(s)		(sizeof(s) - 1)
#define	OFFSET_OF(t, a, b)	(((t *)0)->a - ((t *)0)->b)
#define	FOREVER()			for (;;)
#define	STR_END(s)			((s) + strlen(s) - 1)
#define	STR_ENDX1(s)		((s) + strlen(s))
#define	EQUAL(a, b)			(strcmp((a), (b)) == 0)
#define	EQUALS(a, b)		(strncmp((a), (b), strlen(b)) == 0)
#define	EQUALZ(a, b)		(strncmp((a), (b), STR_SIZE_OF(b)) == 0)
//#define	EQUALN(a, b, n)		(strncmp((a), (b), (n)) == 0)
#define	DIV_MEGA(n)			(((n) + 1024 * 1024 - 1) / (1024 * 1024))
#define	MUL_MEGA(n)			((n) * 1024 * 1024)
#define	NOT(b)				(!(b))

#if defined(_MSC_VER)
#define	CARRIAGE_RETURN()	win32api_console_carriage_return()
#define	CLEAR_LINE()		win32api_console_clear_line()
#define	GETCH()				_getch()
#define	KBHIT()				_kbhit()
#endif

#define	POS(f, r)			(((f) * 10) + (r))
#define	FILE(pos)			(pos / 10)
#define	RANK(pos)			((pos % 10))
#define	VEC(f, r)			(((f) * 10) + (r))
#define	KIND(pc)			((pc) & M_SQ_PIECE)
#define	RAW_KIND(pc)		((pc) & M_SQ_RAW_PC)
#define	IS_ANY_PC(sq)		(((sq) & M_SQ_PIECE) && ((sq) & (B_PC_HITHER | B_PC_YONDER)))
#define	IS_HI_PC(sq)		(((sq) & M_SQ_PIECE) && ((sq) & B_PC_HITHER))
#define	IS_YO_PC(sq)		(((sq) & M_SQ_PIECE) && ((sq) & B_PC_YONDER))
#define	IS_ROCK(sq)			((sq) == SQ_ROCK)
#define	IS_HOLE(sq)			((sq) == SQ_HOLE)
#define	IS_ROOM(sq)			((sq) == SQ_ROOM)
#define	IS_OofB(sq)			((sq) == SQ_OofB)
#define	IS_ONBD(sq)			((sq) & M_SQ_ONBD)
#define	IS_HI_MOVABLE(sq)	(IS_ROOM(sq) || IS_YO_PC(sq))
#define	IS_YO_MOVABLE(sq)	(IS_ROOM(sq) || IS_HI_PC(sq))
#define	IS_THRUABLE(sq)		(IS_ROOM(sq) || IS_HOLE(sq))
#define	IS_HI_TURN(st)		((st) & B_PS_HI_TURN)

#define	FREE_BOARD(bdp) 	((bdp)->next = bdp_free, bdp_free = (bdp), (bdp) = NULL)
#define	FREE_MSEQ(msp) 		((msp)->brop = msp_free, msp_free = (msp))

#if defined(_DEBUG) || (-1)
#define	ASSERT(b) \
	{\
		extern int _getch(void);\
		if (!(b)) {\
			fprintf(stderr, "\n%s(%d): " #b "\n", __FILE__, __LINE__);\
			_getch();\
			exit(255);\
		}\
	}
#else
#define	ASSERT(b)
#endif

#endif // !MACRO_H_

