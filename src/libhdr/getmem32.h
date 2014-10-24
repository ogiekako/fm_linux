/*
 * getmem32.h -- get memory size routines
 * Copyright(c) KAMINA, Jiro 1997.  All rights reserved.
 * $Header: getmem32.hv  1.2  03/11/27 16:13:56  Exp $
*/

#if !defined(GETMEMN32_H_)
#define	GETMEMN32_H_

#include <windows.h>

#define	MS_AVAIL_PHYS		0
#define	MS_TOTAL_PHYS		1
#define	MS_AVAIL_VIRT		2
#define	MS_TOTAL_VIRT		3
#define	MS_AVAIL_SPACE		4
#define	MS_TOTAL_SPACE		5

/* getapm32.c */
extern unsigned long *get_var_mem_size_win32(void);
#if 0
extern unsigned long get_avail_phys_mem_size_win32(void);
extern unsigned long get_total_phys_mem_size_win32(void);
extern unsigned long get_avail_virt_mem_size_win32(void);
extern unsigned long get_total_virt_mem_size_win32(void);
extern unsigned long get_avail_space_size_win32(void);
extern unsigned long get_total_space_size_win32(void);
#endif

#endif  /* !GETMEMN32_H_ */

