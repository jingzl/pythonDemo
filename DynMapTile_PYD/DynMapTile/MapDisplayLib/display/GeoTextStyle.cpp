#include "StdAfx.h"
#include "GeoRender.h"
#include <math.h>

CGeoTextStyle::CGeoTextStyle()
{
	m_Font = NULL;
}
CGeoTextStyle::~CGeoTextStyle()
{
	if(m_Font)
	{
		::DeleteObject(m_Font);
	}
}
void CGeoTextStyle::InitMe(st_txtdisplaymode* pStyle)
{
    if ( m_nID == pStyle->m_nID )
    {
        // 与当前相同，则无须再次初始化
        return;
    }
	m_nID = pStyle->m_nID;
	m_drawtype  = pStyle->m_drawtype ;
	m_nSubCode = pStyle->m_nSubCode; 
	wcscpy(m_szFontName, pStyle->m_szFontName);
	m_nFontSize = pStyle->m_nFontSize;
	m_clrFont = pStyle->m_clrFont;
	m_clrBack = pStyle->m_clrBack;
	m_clrRoundLine = pStyle->m_clrRoundLine; //外围形状颜色
	m_nEdgeWidth = pStyle->m_nEdgeWidth; //边宽
	m_bBold = pStyle->m_bBold;
	m_bUnderLine = pStyle->m_bUnderLine;
	m_bItalic = pStyle->m_bItalic;
	m_bShade = pStyle->m_bShade;
	m_nAlfa = pStyle->m_nAlfa; // 透明度
}

//函数功能: 符号初始化
//输入参数: 无; 
//输出参数: 无
//返回结果: 无
void CGeoTextStyle::InitStyle(CCanvas* pCanvas)
{
	HDC hDC = pCanvas->GetDC();
	m_crold= ::SetTextColor( hDC, m_clrFont );
	m_crBkOld =  ::SetBkColor(hDC, m_clrBack );
	if ( (m_drawtype&eTEXT_DT_TEXTBACKCOLOR) > 0 )
	{
		m_ioldMode = ::SetBkMode(hDC, OPAQUE);
	}
	else
	{
		m_ioldMode = ::SetBkMode(hDC, TRANSPARENT);
	}

    if ( (m_drawtype&eTEXT_DT_STRINGROUND) > 0 )
    {
        GDIColor color;
        // 背景
        color.a = 255;
        color.r = GetRValue(m_clrBack);
        color.g = GetGValue(m_clrBack);
        color.b = GetBValue(m_clrBack);
        m_FillPen1.flags    = 0;
        m_FillPen1.width    = 1;
        m_FillPen1.color    = color;
        m_FillPen1.startCap = Round;
        m_FillPen1.endCap   =Round;
        m_FillPen1.lineJoin = RoundX;

        // 外边框
        color.a = 255;
        color.r = GetRValue(m_clrRoundLine);
        color.g = GetGValue(m_clrRoundLine);
        color.b = GetBValue(m_clrRoundLine);
        m_FillPen2.flags    = 1;
        m_FillPen2.width    = m_nEdgeWidth;
        m_FillPen2.color    = color;
        m_FillPen2.startCap = Round;
        m_FillPen2.endCap   =Round;
        m_FillPen2.lineJoin = RoundX;
    }

	LOGFONT logFont={0};
	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfHeight  = m_nFontSize;
	_tcscpy(logFont.lfFaceName, m_szFontName);
    logFont.lfWeight = m_bBold ? 700 : 400;
    logFont.lfItalic = m_bItalic;
    logFont.lfUnderline = m_bUnderLine;
	if ( m_Font )
	{
		::DeleteObject(m_Font);
	}
	m_Font = ::CreateFontIndirect(&logFont);
	m_holdfont = (HFONT)::SelectObject(hDC, m_Font);
}

void CGeoTextStyle::UnInitStyle(CCanvas* pCanvas)
{
	HDC hDC = pCanvas->GetDC();
    HFONT Font = (HFONT)::SelectObject(hDC, m_holdfont);
    ::DeleteObject(Font);

	::SetTextColor( hDC, m_crold );
	::SetBkColor(hDC, m_crBkOld );
	::SetBkMode(hDC, m_ioldMode);

}
