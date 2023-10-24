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
#include <afxext.h>         // MFC ��չ
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>
// Windows ͷ�ļ�:
#include <windows.h>

#pragma warning ( disable : 4018 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4267 )


#define SYS_CONFIGFILE_PATH ("\\config")
#define SYS_CONFIGFILE_NAME ("\\MapMasterConfig.xml")
// ��ͼ����Ĭ�ϱ���ɫ
#define CN_BACK_COLOR                   RGB(0xC0,0xC0,0xC0)

const unsigned long DRAWMAP_BUFFER_WIDTH = 512;//512;
const unsigned long DRAWMAP_BUFFER_HEIGHT = 512;//512;


//////////////////////////////////////////////////////////////////////////
#include <vector>
#include <map>
#include <list>
#include <algorithm>
using namespace std;

#include "tinyxml.h"
#include "globaldefine.h"
#include "SysPara.h"
#include "MapDisplayCfg.h"
#include "DisplayModeCfg.h"


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

