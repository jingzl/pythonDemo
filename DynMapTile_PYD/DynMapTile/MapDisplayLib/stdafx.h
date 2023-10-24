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
#include "globalfun.h"
#include "IMapDisplay.h"

#include "PngImg.h"
#ifdef _DEBUG
#pragma comment ( lib, "png_d.lib" )
#else
#pragma comment ( lib, "png.lib" )
#endif

#pragma warning (disable : 4018)
#pragma warning (disable : 4244)
#pragma warning (disable : 4267)
#pragma warning (disable : 4786)