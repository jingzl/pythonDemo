#include "StdAfx.h"
#include "GeoRender.h"
#include <math.h>


CGeoPolylineStyle::CGeoPolylineStyle()
{
	m_Length1   = 10;//绘制的长度1
	m_Length2   = 10;//绘制的长度2
	m_Lastdis   = 0;
	m_EdgeWidth = 1;
	m_bAlter    = TRUE;
}

CGeoPolylineStyle::~CGeoPolylineStyle()
{
}

void CGeoPolylineStyle::InitMe(st_linedisplaymode* pStyle)
{
	m_nID      = pStyle->m_nID     ;
	m_drawtype = pStyle->m_drawtype;
	m_clrFore  = pStyle->m_clrFore ;
	m_nWidth   = pStyle->m_nWidth  ;
	m_clrBack  = pStyle->m_clrBack ;
	m_nAlfa    = pStyle->m_nAlfa   ;
}

void CGeoPolylineStyle::InitMeBack(st_linedisplaymode* pStyle)
{
    m_nID      = pStyle->m_nID     ;
    m_drawtype = pStyle->m_drawtype;
    m_clrFore  = pStyle->m_clrBack ;
    m_nWidth   = pStyle->m_nWidth+2;
    m_clrBack  = pStyle->m_clrBack ;
    m_nAlfa    = pStyle->m_nAlfa   ;
}

void CGeoPolylineStyle::InitStyle(CCanvas* pCanvas)
{
	GDIColor color;

	color.a = m_nAlfa;
	color.r = GetRValue(m_clrFore);
	color.g = GetGValue(m_clrFore);
	color.b = GetBValue(m_clrFore);

	m_FillPen1.flags    = GDIPEN_WIDTH|GDIPEN_COLOR|GDIPEN_STARTLINECAP|GDIPEN_ENDLINECAP|GDIPEN_LINEJOIN;
	m_FillPen1.width    = m_nWidth;
	m_FillPen1.color    = color;
	m_FillPen1.startCap = Round;
	m_FillPen1.endCap   =Round;
	m_FillPen1.lineJoin = RoundX;

	color.a = m_nAlfa;
	color.r = GetRValue(m_clrBack);
	color.g = GetGValue(m_clrBack);
	color.b = GetBValue(m_clrBack);
	m_FillPen2.flags    = GDIPEN_WIDTH|GDIPEN_COLOR|GDIPEN_STARTLINECAP|GDIPEN_ENDLINECAP|GDIPEN_LINEJOIN;
	m_FillPen2.width    = m_nWidth;
	m_FillPen2.color    = color;
	m_FillPen2.startCap = Round;
	m_FillPen2.endCap   = Round;
	m_FillPen2.lineJoin = RoundX;

	color.a = m_nAlfa;
	color.r = GetRValue(m_clrFore);
	color.g = GetGValue(m_clrFore);
	color.b = GetBValue(m_clrFore);
	m_EdgePen1.flags    = GDIPEN_WIDTH|GDIPEN_COLOR|GDIPEN_STARTLINECAP|GDIPEN_ENDLINECAP|GDIPEN_LINEJOIN;
	m_EdgePen1.width    = m_nWidth + 2*m_EdgeWidth;
	m_EdgePen1.color    = color;
	m_EdgePen1.startCap = Round;
	m_EdgePen1.endCap   = Round;
	m_EdgePen1.lineJoin = RoundX;

	color.a = m_nAlfa;
	color.r = GetRValue(m_clrFore);
	color.g = GetGValue(m_clrFore);
	color.b = GetBValue(m_clrFore);
	m_EdgePen2.flags    = GDIPEN_WIDTH|GDIPEN_COLOR|GDIPEN_STARTLINECAP|GDIPEN_ENDLINECAP|GDIPEN_LINEJOIN;
	m_EdgePen2.width    = m_nWidth + 2*m_EdgeWidth;
	m_EdgePen2.color    = color;
	m_EdgePen2.startCap = Round;
	m_EdgePen2.endCap   = Round;
	m_EdgePen2.lineJoin = RoundX;

	switch (m_drawtype)
	{
	case eLINE_DT_RAILWAY:		
	case eLINE_DT_SINGLE:
	case eLINE_DT_DOUBLE:
		m_oldpen = pCanvas->SetGdiPen(&m_FillPen1);
		break;
	}
}

void CGeoPolylineStyle::UnInitStyle(CCanvas* pCanvas)
{
	switch (m_drawtype)
	{
	case eLINE_DT_RAILWAY:		
	case eLINE_DT_SINGLE:
	case eLINE_DT_DOUBLE:
		m_oldpen = pCanvas->SetGdiPen(&m_oldpen);
		break;
	}
}
