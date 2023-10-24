
#pragma once

#include <string.h>
#include <wchar.h>
#include "DataType.h"

#if defined(CP_OS_WINCE) || defined(CP_OS_WINPC)			//ceshi 去掉ZeroMemory编译时产生的警告
#include <windows.h>
#include <windef.h>
#include <winnt.h>
#include <basetsd.h>
#include <winbase.h>
#endif
#define CP_ACP      0
#define CP_UTF8     65001

#ifndef wcslen
#define wcslen      WCSLEN
#endif

#ifndef wcscpy
#define wcscpy      WCSCPY
#endif

#ifndef wcsncpy
#define wcsncpy     WCSNCPY
#endif

#ifndef memcpy
#define memcpy      MEMCPY
#endif

#ifndef memmove
#define memmove     MEMMOVE
#endif

#ifndef wcscmp
#define wcscmp      WCSCMP
#endif

#ifndef wcsstr
#define wcsstr      WCSSTR
#endif

#ifndef wcsncmp
#define wcsncmp     WCSNCMP
#endif

#ifndef wcscat
#define wcscat      WCSCAT
#endif

#ifndef ZeroMemory
#define ZeroMemory(a,b) memset((a),0x00,(b))
#endif

#ifdef __cplusplus
extern "C"
{
#endif
	/*
	ZWCHAR* WCSCPY(ZWCHAR* dst, const ZWCHAR* src);
	ZVOID*  MEMCPY (ZVOID* dst, const ZVOID* src, size_t count);
	ZWCHAR* WCSNCPY(ZWCHAR* dest, const ZWCHAR * source, size_t count);
	ZVOID*  MEMMOVE (ZVOID* dst, const ZVOID* src, size_t count);

	ZINT32  WCSCMP(const ZWCHAR *src, const ZWCHAR *dst);
	ZINT32  WCSNCMP(const ZWCHAR* first, const ZWCHAR* last, size_t count);
	size_t  WCSLEN(const ZWCHAR *ws);

	ZWCHAR* WCSSTR(const ZWCHAR* wcs1, const ZWCHAR* wcs2);
	ZWCHAR* WCSCAT(ZWCHAR *dst, const ZWCHAR *src);
    */
#ifdef __cplusplus
}
#endif

ZWCHAR* WCSCPY(ZWCHAR* dst, const ZWCHAR* src);
ZVOID*  MEMCPY (ZVOID* dst, const ZVOID* src, size_t count);
ZWCHAR* WCSNCPY(ZWCHAR* dest, const ZWCHAR * source, size_t count);
ZVOID*  MEMMOVE (ZVOID* dst, const ZVOID* src, size_t count);

ZINT32  WCSCMP(const ZWCHAR *src, const ZWCHAR *dst);
ZINT32  WCSNCMP(const ZWCHAR* first, const ZWCHAR* last, size_t count);
size_t  WCSLEN(const ZWCHAR *ws);

ZWCHAR* WCSSTR(const ZWCHAR* wcs1, const ZWCHAR* wcs2);
ZWCHAR* WCSCAT(ZWCHAR *dst, const ZWCHAR *src);

size_t  MBSTOWCS(ZINT32 codePage, ZWCHAR* pwcs, const ZCHAR* ps, size_t n);
size_t  WCSTOMBS(ZINT32 codePage, ZCHAR* ps, const ZWCHAR* pwcs, size_t n);
size_t  WSPRINTF(ZWCHAR *wstr, const ZWCHAR *format, ...);
