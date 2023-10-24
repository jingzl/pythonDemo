#include "StdAfx.h"
#include "GeoRender.h"
#include <math.h>

CGeoPolygonStyle::CGeoPolygonStyle()
{
	//初始化多边形绘制
}

CGeoPolygonStyle::~CGeoPolygonStyle()
{
}

void CGeoPolygonStyle::InitMe(st_areadisplaymode* pStyle)
{
    if ( m_nID == pStyle->m_nID )
    {
        return;
    }
    m_nID        = pStyle->m_nID;
    m_drawtype   = pStyle->m_drawtype;
    m_clrFill    = pStyle->m_clrFill;
    m_nEdgeWidth = pStyle->m_nEdgeWidth;
    m_clrEdge    = pStyle->m_clrEdge;
    m_nAlfa      = pStyle->m_clrEdge;
    _tcscpy(m_szFillImgPath, pStyle->m_szFillImgPath);	
}

void CGeoPolygonStyle::InitStyle(CCanvas* pCanvas)
{
    //用用户设定值初始化符号
	//用用户值初始化
	switch ( m_drawtype )
	{
	case eAREA_DT_SOLID:
		{
            //颜色填充
			GDIColor color;
			color.a = m_nAlfa;
			color.r = GetRValue(m_clrEdge);
			color.g = GetGValue(m_clrEdge);
			color.b = GetBValue(m_clrEdge);
			m_EdgePen.flags = GDIPEN_WIDTH|GDIPEN_COLOR|GDIPEN_STARTLINECAP|GDIPEN_ENDLINECAP|GDIPEN_LINEJOIN;
			m_EdgePen.color = color;
			m_EdgePen.width = m_nEdgeWidth;
			m_EdgePen.startCap = Round;
			m_EdgePen.endCap   = Round;
			m_EdgePen.lineJoin = RoundX;
			m_oldpen=pCanvas->SetGdiPen(&m_EdgePen);		
			m_OldClor=pCanvas->SetGdiBrushColor(m_clrFill);

            break;
		}
	case eAREA_DT_BMP:
		{
            break;
		}
	}
}

void CGeoPolygonStyle::UnInitStyle(CCanvas* pCanvas)
{
    pCanvas->SetGdiPen(&m_oldpen);
    pCanvas->SetGdiBrushColor(m_OldClor);
}
