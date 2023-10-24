// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include <afx.h>
#include <afxwin.h>         // MFC 核心组件和标准组件
#include <windows.h>


#include "IMapOutput.h"
#pragma warning ( disable : 4018 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4267 )


inline int UnicodeToUTF8( WCHAR* wcstr, char* mbstr, int count )
{
    return WideCharToMultiByte( CP_UTF8, 0, wcstr, -1, mbstr, count, NULL, NULL );
}

inline wstring str2wstr( const string& str )
{
    const char* _Source = str.c_str();
    size_t _Dsize = str.size() + 1;
    wchar_t *_Dest = new wchar_t[_Dsize];
    wmemset(_Dest, 0, _Dsize);
    MultiByteToWideChar( CP_ACP, 0, _Source, -1, _Dest, _Dsize );
    wstring result = _Dest;
    delete [] _Dest;
    _Dest = NULL;
    return result;
}
