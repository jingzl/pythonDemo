#pragma once 

#if defined(__CYGWIN__)
#define CP_OS_CYGWIN
#elif defined(_WIN32_WCE)
#define CP_OS_WINCE
#elif defined(_WIN32) || defined(__WIN32__)
#define CP_OS_WINPC
#else
#define CP_OS_LINUX
#endif

#ifndef _INT8_DEFINED
#define _INT8_DEFINED
typedef char                ZINT8;
#endif // !_INT8_DEFINED

#ifndef _UINT8_DEFINED
#define _UINT8_DEFINED
typedef unsigned char       ZUINT8;
#endif // !_UINT8_DEFINED

#ifndef _INT16_DEFINED
#define _INT16_DEFINED
typedef short               ZINT16;
#endif  // !_INT16_DEFINED

#ifndef _UINT16_DEFINED
#define _UINT16_DEFINED
typedef unsigned short      ZUINT16;
#endif // !_UINT16_DEFINED

#ifndef _INT32_DEFINED
#define _INT32_DEFINED
typedef int                 ZINT32;
#endif // !_INT32_DEFINED

#ifndef _UINT32_DEFINED
#define _UINT32_DEFINED
typedef unsigned int        ZUINT32;
#endif // !_UINT32_DEFINED

#ifndef _INT64_DEFINED
#define _INT64_DEFINED
typedef long long int       ZINT64;
#endif // !_INT64_DEFINED

#ifndef _UINT64_DEFINED
#define _UINT64_DEFINED
typedef unsigned long long int   ZUINT64;
#endif // !_UINT64_DEFINED

#ifndef _FLOAT32_DEFINED
#define _FLOAT32_DEFINED
typedef float               ZFLOAT32;
#endif // !_FLOAT32_DEFINED

#ifndef _FLOAT64_DEFINED
#define _FLOAT64_DEFINED
typedef double              ZFLOAT64;
#endif // !_FLOAT64_DEFINED

typedef ZINT8               ZCHAR;
typedef ZUINT8              ZBYTE;
typedef ZINT32              ZBOOL;
typedef ZUINT8              ZUCHAR;
typedef ZUINT16             ZWORD;

typedef wchar_t             ZWCHAR;
typedef ZUINT32             ZDWORD;
typedef ZUINT32*            ZLPDWORD;
typedef ZINT64              ZLONGLONG;
typedef ZUINT64             ZULONGLONG;
typedef const ZCHAR*        ZLPCSTR;
typedef ZWCHAR*             ZLPWSTR;
typedef const ZWCHAR*       ZLPCWSTR;
typedef ZWCHAR              ZTCHAR;
typedef ZTCHAR*             ZLPTSTR;
typedef const ZTCHAR*       ZLPCTSTR;

#ifndef _ZVOID_DEFINED
#define _ZVOID_DEFINED
typedef void                ZVOID;
#endif // !_ZVOID_DEFINED

#ifndef _ZPVOID_DEFINED
#define _ZPVOID_DEFINED
typedef void*               ZPVOID;
#endif // !_ZPVOID_DEFINED

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif


typedef struct _ZNFloatSection
{
    ZFLOAT32 *array;
    ZUINT32 num;

    _ZNFloatSection()
    {
        array = NULL;
        num = 0;
    }
    ~_ZNFloatSection()
    {
        if ( array != NULL )
        {
            delete [] array;
            array = NULL;
        }
    }

}ZNFloatSection;

typedef struct _ZNInt8Section
{
    ZINT8 *array;
    ZUINT16 num;

    _ZNInt8Section()
    {
        array = NULL;
        num = 0;
    }
    ~_ZNInt8Section()
    {
        if ( array != NULL )
        {
            delete [] array;
            array = NULL;
        }
    }

}ZNInt8Section;

typedef struct _ZNInt16Section
{
    ZINT16 *array;
    ZUINT32 num;

    _ZNInt16Section()
    {
        array = NULL;
        num = 0;
    }
    ~_ZNInt16Section()
    {
        if ( array != NULL )
        {
            delete [] array;
            array = NULL;
        }
    }

}ZNInt16Section;


//add by zbx 2010-5-19
#ifndef _SUCCESS_DEFINED
#define _SUCCESS_DEFINED
#undef SUCCESS
#define SUCCESS		         0
#endif

#ifndef _INVALID_DEFINED
#define _INVALID_DEFINED
#undef INVALID
#define INVALID		        0xFFFFFFFF
#endif

#ifndef _EFAILED_DEFINED
#define _EFAILED_DEFINED
#undef EFAILED
#define EFAILED		         1
#endif

#ifndef _ZCOLORREF_DEFINED
#define _ZCOLORREF_DEFINED
typedef unsigned long		 ZCOLORREF;
#endif

// Added by PYF 2010/05/25

#define ZTRUE               1
#define ZFALSE              0

typedef ZPVOID              ZHandle;
typedef void* (*PTHREAD_START) (void *);

#if defined(CP_OS_LINUX) || defined(CP_OS_CYGWIN)
#define ZWINAPI
#define ZCALLBACK
#else
#define ZWINAPI  __stdcall
#define ZCALLBACK __stdcall
#endif

#define ZSTATUS_WAIT_0                    ((ZINT32)0x00000000L) 
#define ZSTATUS_ABANDONED_WAIT_0          ((ZINT32)0x00000080L) 
#define ZWAIT_OBJECT_0                    ((ZSTATUS_WAIT_0 ) + 0)
#define ZWAIT_ABANDONED                   ((ZSTATUS_ABANDONED_WAIT_0 ) + 0 )
#define ZWAIT_FAILED                      ((ZINT32)INVALID)
#define ZWAIT_TIMEOUT                     258L

#define ZCREATE_ACTIVE                     0x00000000
#define ZCREATE_SUSPENDED                  0x00000004

#ifndef CP_COOLMAP_API
  #if defined(CP_OS_LINUX) || defined(CP_OS_CYGWIN)
     #define CP_COOLMAP_API
  #else
     #ifdef CP_COOLMAP_EXPORTS
        #define CP_COOLMAP_API __declspec(dllexport)
     #else
        #define CP_COOLMAP_API __declspec(dllimport)
     #endif
  #endif
#endif

#if defined(CP_OS_LINUX) || defined(CP_OS_CYGWIN)
#include "jni.h"
typedef struct tag_JNIPARA
{
	JavaVM* m_jvm;
	jobject m_object;
}JNIPara;
#else
typedef ZVOID JNIPara;
#endif

