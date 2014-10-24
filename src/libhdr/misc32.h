/*
 * misc32.h -- get memory size routines
 * Copyright(c) KAMINA, Jiro 1997.  All rights reserved.
 * $Header: misc32.hv  1.2  03/11/27 16:15:14  Exp $
*/

#if !defined(MISC32_H_)
#define	MISC32_H_

/* delay32.c */
extern void delay_win32(unsigned int d);

/* rs232c32.c */
extern int RS_init(void);
extern void RS_close(void);
extern int RS_read(unsigned char *s);
extern void RS_send(unsigned char c);

#endif  /* !MISC32_H */

