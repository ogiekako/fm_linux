/*
 * consjk.h -- Jiro Kamina original cursor manipulate sub-routine header
 * Copyright(c) Jiro Kamina 1995-1998.  All rights reserved.
 * $Header: consjk.hv  1.1  98/09/22 12:04:18  Exp $
*/

#if !defined(CONSJK_H_)
#define	CONSJK_H_

/* getcp98.c */
extern int getch_pc98(void);

/* getcpjk.c */
extern void get_cursor_position_dos(FILE *out, int *y, int *x);

/* kbhit98.c */
extern int kbhit_pc98(void);

#endif  /* !CONSJK_H_ */

