#include "stdafx.h"
#include "DisplayInfo.h"
#include "LabelAdjust.h"
#include "PointLabelAdjust.h"


CDisplayInfo::CDisplayInfo(void):
m_pCanvas(NULL),
m_lScale(0),
m_dfScaleFactor(0.0f),
m_pPoints(NULL)
{
    // 初始值
    // 设备相关参数
	//HDC hDC = GetDC(NULL);
	//m_dPixelPerMeter = 39.37007874*(GetDeviceCaps(hDC, LOGPIXELSX));
	//ReleaseDC(NULL, hDC);

    // 设置无关参数
    m_dPixelPerMeter = 3792.5925925925926;

    m_ptPixelCenter.x = 0;
    m_ptPixelCenter.y = 0;

	memset(m_szImagePath, 0, sizeof(TCHAR)*MAX_PATH);
	m_pLabelAdjust = NULL;
    m_pPointLabelAdjust = NULL;

    m_bShow3D = FALSE;
}

CDisplayInfo::~CDisplayInfo(void)
{
	if(m_pCanvas)
	{
		delete m_pCanvas;
		m_pCanvas = NULL;
	}
	if(m_pPoints)
	{
		free(m_pPoints);
		m_pPoints = NULL;
	}
	if(m_pLabelAdjust)
	{
		m_pLabelAdjust->Release();
		delete m_pLabelAdjust;
		m_pLabelAdjust = NULL;
	}
    if(m_pPointLabelAdjust)
    {
        m_pPointLabelAdjust->Release();
        delete m_pPointLabelAdjust;
        m_pPointLabelAdjust = NULL;
    }
}

BOOL CDisplayInfo::Initialize(SIZE bufSize)
{
	m_bufSize = bufSize;
	if(!m_pPoints)
	{
		m_pPoints = (POINT*) malloc(sizeof(POINT)*409600);
	}
	if(m_pCanvas)
	{
		delete m_pCanvas;
		m_pCanvas = NULL;
	}

	if(m_pLabelAdjust)
	{
		m_pLabelAdjust->Release();
	}
	else
	{
		m_pLabelAdjust = new CLabelAdjust();
	}
	if(m_pLabelAdjust)
	{
		m_pLabelAdjust->Create(bufSize.cx, bufSize.cy);
	}
	else
	{
		return FALSE;
	}

    if(m_pPointLabelAdjust)
    {
        m_pPointLabelAdjust->Release();
    }
    else
    {
        m_pPointLabelAdjust = new CPointLabelAdjust();
    }
    if (m_pPointLabelAdjust)
    {
        m_pPointLabelAdjust->Create(bufSize.cx, bufSize.cy);
    }
    else
    {
        return FALSE;
    }

	m_pCanvas = new CCanvas();
	if ( m_pCanvas )
	{
        //Start3DMode();
		return m_pCanvas->Create(bufSize.cx, bufSize.cy);
	}
	else
	{
		return FALSE;
	}
}

void CDisplayInfo::SetImagePath(const TCHAR* lpszPath)
{
	if(lpszPath && _tcslen(lpszPath)<MAX_PATH)
	{
		_tcscpy(m_szImagePath, lpszPath);
	}
}

HDC CDisplayInfo::GetHDC()
{
	if(m_pCanvas)
	{
		return m_pCanvas->GetDC();
	}
	else
	{
		return NULL;
	}
	
}

BOOL CDisplayInfo::SetMapPos(GEOPOINT pos, _tag_map_pos mapPos)
{
	if(!m_pCanvas)
	{
		return FALSE;
	}
    m_ptCenter = pos;
    int x,y;
    m_ProjectTrans.LongLatToPixelXY(pos.x, pos.y, m_lScale, x, y);

	switch (mapPos)
	{
	case MP_CENTER:	
		m_ptLB.x = x - m_bufSize.cx/2;
		m_ptLB.y = y + m_bufSize.cy/2;
		break;
	case MP_LEFT_BOTTOM:
        m_ptLB.x = x;
        m_ptLB.y = y;
		break;
	case MP_LEFT_TOP:
        m_ptLB.x = x;
        m_ptLB.y = y+m_bufSize.cy;
		break;
	}

    m_ptPixelCenter.x = x;
    m_ptPixelCenter.y = y;

	return ResetMap();
}

BOOL CDisplayInfo::SetMapScale(double dfScale, int scale)
{
	if(!m_pCanvas)
	{
		return FALSE;
	}
    m_lScale = scale;
    int x,y;
    m_ProjectTrans.LongLatToPixelXY(m_ptCenter.x, m_ptCenter.y, m_lScale, x, y);
    m_ptPixelCenter.x = x;
    m_ptPixelCenter.y = y;

    x -= m_bufSize.cx/2;
    y += m_bufSize.cy/2;
	m_ptLB.x = x;
	m_ptLB.y = y;

	return ResetMap();
}

BOOL CDisplayInfo::MoveMap(double dfOffsetX, double dfOffsetY)
{
	if(!m_pCanvas)
	{
		return FALSE;
	}

    // 根据中心点
    m_ptPixelCenter.x += dfOffsetX;
    m_ptPixelCenter.y -= dfOffsetY;
    m_ptLB.x = m_ptPixelCenter.x - m_bufSize.cx/2;
    m_ptLB.y = m_ptPixelCenter.y + m_bufSize.cy/2;
    m_ProjectTrans.PixelXYToLongLat(m_ptPixelCenter.x, m_ptPixelCenter.y, m_lScale, m_ptCenter.x, m_ptCenter.y);

	/*
    // 根据LB点
    m_ptLB.x = m_ptLB.x + dfOffsetX;
	m_ptLB.y = m_ptLB.y - dfOffsetY;
    m_ProjectTrans.PixelXYToLongLat(m_ptLB.x+m_bufSize.cx/2, m_ptLB.y-m_bufSize.cy/2, m_lScale, m_ptCenter.x, m_ptCenter.y);
    */

	return ResetMap();
}

BOOL CDisplayInfo::GetLLPos( POINT& pt, GEOPOINT& geopt, int& nCol, int& nRow )
{
    POINT ptPos;
    ptPos.x = pt.x + m_ptLB.x;
    ptPos.y = pt.y + m_ptLB.y - m_bufSize.cy;
    m_ProjectTrans.PixelXYToLongLat( ptPos.x, ptPos.y, m_lScale, geopt.x, geopt.y );
    pt = ptPos;

    bool bx = false;
    bool by = false;
    m_ProjectTrans.pixelXYToTileXY( ptPos.x, ptPos.y, nCol, nRow, bx, by );

    return TRUE;
}

BOOL CDisplayInfo::Src2Map( POINT& ptSrc, GEOPOINT& geoPt )
{
    POINT ptPos;
    ptPos.x = ptSrc.x + m_ptLB.x;
    ptPos.y = ptSrc.y + m_ptLB.y - m_bufSize.cy;
    m_ProjectTrans.PixelXYToLongLat( ptPos.x, ptPos.y, m_lScale, geoPt.x, geoPt.y );
    return TRUE;
}

GEOPOINT CDisplayInfo::GetCenterPos()
{
    return m_ptCenter; 
}

GEORECT CDisplayInfo::GetMapRect()
{
	return m_geoRect;
}

BOOL CDisplayInfo::ResetMap()
{
	if(!m_pCanvas)
	{
		return FALSE;
	}

    m_ProjectTrans.PixelXYToLongLat(m_ptLB.x, m_ptLB.y, m_lScale, m_geoRect.dMinX, m_geoRect.dMinY);
    m_ProjectTrans.PixelXYToLongLat(m_ptLB.x+m_bufSize.cx, m_ptLB.y-m_bufSize.cy, m_lScale, m_geoRect.dMaxX, m_geoRect.dMaxY);

	return TRUE;
}


void CDisplayInfo::Start3DMode()
{
    m_bShow3D   = TRUE;
    //SetXYOffset(0.5, 0.1);
    POINT points[5] = {{0,0},{0,0},{0,0},{0,0},{0,0}};
#if 0
    points[0].x = -480;
    points[0].y = 160;
    points[1].x = 720;
    points[1].y = 160;
    points[2].x = 240;
    points[2].y = 380;
    points[3].x = 0;
    points[3].y = 380;
#else
    /*
    points[0].x = -640;
    points[0].y = 350;
    points[1].x = 960;
    points[1].y = 350;
    points[2].x = 320;
    points[2].y = 610;
    points[3].x = 0;
    points[3].y = 610;
    */
    points[0].x = -240;
    points[0].y = 0;
    points[1].x = 720;
    points[1].y = 0;
    points[2].x = 480;
    points[2].y = 700;
    points[3].x = 0;
    points[3].y = 700;

    /*
    points[0].x = -960;
    points[0].y = 400;
    points[1].x = 1440;
    points[1].y = 400;
    points[2].x = 480;
    points[2].y = 900;
    points[3].x = 0;
    points[3].y = 900;
    */
#endif

    if ( m_pCanvas != NULL )
    {
        SetAggTransform(m_pCanvas->GetGDIHandle(), GetBufRect(), points, (double*)m_AffineTrans.m_mx);
    }
}

void CDisplayInfo::End3DMode()
{
    m_bShow3D   = FALSE;
    //SetXYOffset(0.5, 0.5);
    if(m_pCanvas)
    {
        SetAggTransform(m_pCanvas->GetGDIHandle(), GetBufRect(), NULL, NULL);
    }
}