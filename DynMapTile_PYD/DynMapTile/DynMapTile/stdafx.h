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
#include <afxext.h>         // MFC 扩展
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>
// Windows 头文件:
#include <windows.h>

#pragma warning ( disable : 4018 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4267 )


#define SYS_CONFIGFILE_PATH ("\\config")
#define SYS_CONFIGFILE_NAME ("\\MapMasterConfig.xml")
// 地图窗口默认背景色
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

