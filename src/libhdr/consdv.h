/*
 * consdv.h -- DOS/V cursor manipulate sub-routine header
 * Copyright(c) Jiro Kamina 1997.  All rights reserved.
 * $Header: consdv.hv  1.1  98/09/22 12:04:18  Exp $
*/

#if !defined(CONSDV_H_)
#define	CONSDV_H_

/* erseosdv.c */
extern void erase_eos_dosv(void);

/* curondv.c */
extern void cursor_on_dosv(void);
extern void cursor_off_dosv(void);

#endif  /* !CONSDV_H_ */

