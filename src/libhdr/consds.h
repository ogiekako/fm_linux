/*
 * consds.h -- original DOS cursor manipulate sub-routine header
 * Copyright(c) Jiro Kamina 1995-1998.  All rights reserved.
 * $Header: consds.hv  1.1  98/11/21 12:25:28  Exp $
*/

#if !defined(CONSDS_H_)
#define	CONSDS_H_

#if defined(__TURBOC__)
#include <conio.h>
#endif

#if defined(__CYGWIN32__)
#define	getch()		_getch()
#define	kbhit()		_kbhit()
#elif defined(__GNUC__)
#define	getch()		getch_pc98()
#define	kbhit()		kbhit_pc98()
#endif

#if defined(__cplusplus)
extern "C" {
#endif

/* curondv.c */
extern void cursor_on_dosv(void);
extern void cursor_off_dosv(void);

/* erseosdv.c */
extern void erase_eos_dosv(void);
extern void erase_eos_e_dosv(void);

/* getcpjk.c */
extern void get_cursor_position_dos(FILE *out, int *y, int *x);

/* getcp98.c */
extern int getch_pc98(void);

/* kbhit98.c */
extern int kbhit_pc98(void);

#if defined(__cplusplus)
}
#endif

#endif  /* !CONSDS_H_ */

