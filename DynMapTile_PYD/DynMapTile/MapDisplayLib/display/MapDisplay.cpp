#include "StdAfx.h"
#include "MapDisplay.h"


TCHAR g_szConfigPath[MAX_PATH] = {0};

int GetBinPath(TCHAR* path)
{
    // 返回外部传递的路径
    if ( _tcslen(g_szConfigPath) >= 0 )
    {
        _tcscpy(path, g_szConfigPath);
        return wcslen(g_szConfigPath);
    }
    else
    {
        return 0;
    }

#if 0
	TCHAR *pToken;
	TCHAR szNavPath[MAX_PATH] = {0};

	// 初始化程序加载路径
	int nRet = GetModuleFileName(NULL, szNavPath, MAX_PATH);
	if (nRet < 0)
		return -1;

	if ((pToken = wcsrchr(szNavPath, '\\')) != NULL)
	{
		*pToken = '\0';
	}
	if(path == NULL)
	{
		return wcslen(szNavPath);
	}
	else
	{
		wcscpy(path, szNavPath);
		return wcslen(szNavPath);
	}
#endif
}

IMapDisplay* CreateMapDisplay()
{
	return (IMapDisplay*) new CMapDisplay;
}

CMapDisplay::CMapDisplay(void)
{	
}

CMapDisplay::~CMapDisplay(void)
{
}

BOOL CMapDisplay::Initialize(SIZE bufSize)
{
	return m_displayInfo.Initialize(bufSize);
}

void CMapDisplay::SetImagePath(const TCHAR* lpszPath)
{
	if(lpszPath && _tcslen(lpszPath)<MAX_PATH)
	{
        _tcscpy( g_szConfigPath, lpszPath );
		m_displayInfo.SetImagePath(lpszPath);
	}
}

BOOL CMapDisplay::SetMapPos(GEOPOINT pos, _tag_map_pos mapPos)
{
	return m_displayInfo.SetMapPos(pos, mapPos);
}

BOOL CMapDisplay::SetMapScale(double dfScale, int scale)
{
	return m_displayInfo.SetMapScale(dfScale, scale);
}

BOOL CMapDisplay::MoveMap(double dfOffsetX, double dfOffsetY)
{
	return m_displayInfo.MoveMap(dfOffsetX, dfOffsetY);
}

BOOL CMapDisplay::GetLLPos( POINT& pt, GEOPOINT& geopt, int& nCol, int& nRow )
{
    return m_displayInfo.GetLLPos( pt, geopt, nCol, nRow );
}

GEORECT CMapDisplay::GetMapRect()
{
	return m_displayInfo.GetMapRect();
}

GEOPOINT CMapDisplay::GetCenterPos()
{
    return m_displayInfo.GetCenterPos();
}

BOOL CMapDisplay::DrawMap(DWORD dwBackColor, PMAP_DISP_DATA pData, HDC* pDC, PTILETEXT_INFO pTileTextInfo, BOOL bGridLine )
{
	return m_mapRender.DrawMap(&m_displayInfo, dwBackColor, pData, pDC, pTileTextInfo, bGridLine );
}

void CMapDisplay::Release()
{
	delete this;
}

