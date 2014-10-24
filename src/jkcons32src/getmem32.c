/*
 * getmem32.c -- get various memory size on Win32
 * Copyright(c) KAMINA, Jiro 1997.  All rights reserved.
 * $Header: getmem32.cv  1.2  03/11/27 12:27:17  Exp $
*/

static char copyright[] =
#if 0
	"get_var_mem_size_win32(), "
	"get_avail_phys_mem_size_win32(), "
	"get_total_phys_mem_size_win32(), "
	"get_avail_virt_mem_size_win32(), "
	"get_total_virt_mem_size_win32(), "
	"get_avail_space_size_win32(), "
	"get_total_space_size_win32(): "
#else
	"get_var_mem_size_win32(): "
#endif
	"COPYRIGHT(c) KAMINA, Jiro 1997 "
	"- $Header: getmem32.cv  1.2  03/11/27 12:27:17  Exp $";

#include <windows.h>
#include <stdio.h>

#include "getmem32.h"

unsigned long *
get_var_mem_size_win32(void)
{
	MEMORYSTATUS memoryStatus;
	static unsigned long mem_size[6];

	memset(&memoryStatus, '\0', sizeof(memoryStatus));
	GlobalMemoryStatus(&memoryStatus);
	mem_size[MS_AVAIL_PHYS] = memoryStatus.dwAvailPhys;
	mem_size[MS_TOTAL_PHYS] = memoryStatus.dwTotalPhys;
	mem_size[MS_AVAIL_VIRT] = memoryStatus.dwAvailPageFile;
	mem_size[MS_TOTAL_VIRT] = memoryStatus.dwTotalPageFile;
	mem_size[MS_AVAIL_SPACE] = memoryStatus.dwAvailVirtual;
	mem_size[MS_TOTAL_SPACE] = memoryStatus.dwTotalVirtual;
	return mem_size;
}

unsigned long
get_avail_phys_mem_size_win32(void)
{
	MEMORYSTATUS memoryStatus;

	memset(&memoryStatus, '\0', sizeof(memoryStatus));
	GlobalMemoryStatus(&memoryStatus);
	return memoryStatus.dwAvailPhys;
}

#if 0
unsigned long
get_total_phys_mem_size_win32(void)
{
	MEMORYSTATUS memoryStatus;

	memset(&memoryStatus, '\0', sizeof(memoryStatus));
	GlobalMemoryStatus(&memoryStatus);
	return memoryStatus.dwTotalPhys;
}

unsigned long
get_avail_virt_mem_size_win32(void)
{
	MEMORYSTATUS memoryStatus;

	memset(&memoryStatus, '\0', sizeof(memoryStatus));
	GlobalMemoryStatus(&memoryStatus);
	return memoryStatus.dwAvailPageFile;
}

unsigned long
get_total_virt_mem_size_win32(void)
{
	MEMORYSTATUS memoryStatus;

	memset(&memoryStatus, '\0', sizeof(memoryStatus));
	GlobalMemoryStatus(&memoryStatus);
	return memoryStatus.dwPageFile;
}

unsigned long
get_avail_space_size_win32(void)
{
	MEMORYSTATUS memoryStatus;

	memset(&memoryStatus, '\0', sizeof(memoryStatus));
	GlobalMemoryStatus(&memoryStatus);
	return memoryStatus.dwAvailVirtual;
}

unsigned long
get_total_space_size_win32(void)
{
	MEMORYSTATUS memoryStatus;

	memset(&memoryStatus, '\0', sizeof(memoryStatus));
	GlobalMemoryStatus(&memoryStatus);
	return memoryStatus.dwTotalVirtual;
}
#endif

