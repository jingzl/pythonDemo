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