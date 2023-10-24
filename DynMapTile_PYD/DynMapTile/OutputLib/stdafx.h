// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include <afx.h>
#include <afxwin.h>         // MFC ��������ͱ�׼���
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
