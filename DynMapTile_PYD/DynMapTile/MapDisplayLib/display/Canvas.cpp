#include "StdAfx.h"
#include <math.h>
#include "Canvas.h"
#include "globalfun.h"


#define DISTANCE(PT1, PT2)  (sqrt(double( ( (PT1).x-(PT2).x)*((PT1).x-(PT2).x) ) + ( ( (PT1).y-(PT2).y)*((PT1).y-(PT2).y)) ))

static int WTOA(LPSTR mbstr, LPCTSTR wcstr, int count)
{
	int nRet = 0;
	if (!(wcstr && *wcstr != '\0') || !mbstr || (count <= 0))
		return 0;

	nRet = WideCharToMultiByte(CP_ACP, 0, wcstr, -1, mbstr, count - 1, NULL, NULL);
	return nRet;
}

CCanvas::CCanvas()
{	
	m_bCreated = FALSE;	

	m_GDIHandle=NULL;

	m_GDIPen.flags=GDIPEN_WIDTH|GDIPEN_COLOR|GDIPEN_STARTLINECAP|GDIPEN_ENDLINECAP|GDIPEN_LINEJOIN;
	m_GDIPen.width=1;
	m_GDIPen.color.a=0xff;
	m_GDIPen.color.r=0;
	m_GDIPen.color.g=0;
	m_GDIPen.color.b=0;
	m_GDIPen.startCap=Round;
	m_GDIPen.endCap=Round;
	m_GDIPen.lineJoin=RoundX;

	m_GDIColor.a=0xff;
	m_GDIColor.r=0xff;
	m_GDIColor.g=0xff;
	m_GDIColor.b=0xff;

	m_dirArrowAttri.headLength = 30;
	m_dirArrowAttri.headWidth = 28;
	m_dirArrowAttri.tailLength = 60;
	m_dirArrowAttri.tailWidth1 = 14;
	m_dirArrowAttri.tailWidth2 = 28;

	m_hMemDC  = NULL;
	m_pPoints = NULL;

    ZeroMemory( &m_minPt, sizeof(POINT) );
    ZeroMemory( &m_maxPt, sizeof(POINT) );
    m_lpLineList = new int[64];
    ZeroMemory( &m_rcClip, sizeof(RECT) );
}

CCanvas::~CCanvas()
{
    if ( m_lpLineList != NULL )
    {
        delete [] m_lpLineList;
        m_lpLineList = NULL;
    }

	Release();
}

BOOL CCanvas::Create(int iWidth, int iHeight)
{
	if(m_bCreated)
		return FALSE;

	HDC hScreenDC = ::GetDC(NULL);

	if(!hScreenDC)
		return FALSE;	

	if( !(m_hMemDC=CreateCompatibleDC(hScreenDC)) )
	{
		::ReleaseDC(NULL, hScreenDC);
		return FALSE;
	}

	::ReleaseDC(NULL, hScreenDC);

	CHAR szPath[MAX_PATH];

	TCHAR *pToken, szValue[256] = {0};
	TCHAR szNavPath[MAX_PATH] = {0};
	// 初始化程序加载路径
	int nRet = GetModuleFileName(NULL, szNavPath, MAX_PATH);
	if (nRet < 0)
		return FALSE;

	if ((pToken = wcsrchr(szNavPath, '\\')) != NULL)
	{
		*pToken = '\0';
		if ((pToken = wcsrchr(szNavPath, '\\')) != NULL)
		{
			*pToken = '\0';
		}
	}
	WTOA(szPath, szNavPath, MAX_PATH-1);
	strcat(szPath, "\\configdat\\IMAGES\\");
	m_GDIHandle=GetGDI(m_hMemDC,iWidth,iHeight,szPath);
	if(!m_GDIHandle)
	{
		::DeleteDC(m_hMemDC);
		m_hMemDC=NULL;

		::ReleaseDC(NULL, hScreenDC);
		return FALSE;
	}

	ClearGDI(m_GDIHandle,m_GDIColor);
	m_bCreated = TRUE;

	m_nWidth = iWidth;
	m_nHeight = iHeight;	

	if(!m_pPoints)
	{
		m_pPoints = (POINT*) malloc(sizeof(POINT)*409600);
	}

	return TRUE;
}

void CCanvas::Release()
{
	if(m_GDIHandle)
	{		
		DeleteGDI(m_GDIHandle);
		m_GDIHandle = NULL;
	}

	if(m_hMemDC)
	{
		::DeleteDC(m_hMemDC);
		m_hMemDC = NULL;	
	}	

	if(m_pPoints)
	{
		free(m_pPoints);
		m_pPoints = NULL;
	}

	m_bCreated = FALSE;
}

BOOL CCanvas::Blt(CCanvas *pSrcsurface, LPRECT pDestRect, LPRECT pSrcRect, DWORD dwRop)
{
	if(!m_bCreated)
		return FALSE;
	if(!pSrcsurface)
		return FALSE;	

	return BitBltGDI(m_GDIHandle,(GDIRect)(*pDestRect),(GDIRect)(*pSrcRect));	
}

void CCanvas::Refresh(HDC hdc, int iOffsetX, int iOffsetY, LPRECT lpRect)
{
	RECT  tmpRect;	

	if(!m_bCreated)
		return;	

	if(!lpRect)
	{
		tmpRect.left= 0;
		tmpRect.top = 0;
		tmpRect.right = m_nWidth;
		tmpRect.bottom = m_nHeight;
		lpRect = &tmpRect;		
	}		

	::BitBlt(hdc, lpRect->left, lpRect->top, 
		lpRect->right-lpRect->left, 
		lpRect->bottom-lpRect->top,
		m_hMemDC, iOffsetX, iOffsetY, SRCCOPY);

}

GDIPen   CCanvas::SetGdiPen(GDIPen* pen)
{
	GDIPen old;

	old=m_GDIPen;
	m_GDIPen=*pen;

	return old;
}

COLORREF CCanvas::SetGdiBrushColor(COLORREF color)
{
	COLORREF old;

	old=RGB(m_GDIColor.r,m_GDIColor.g,m_GDIColor.b);

	m_GDIColor.a=0xff;
	m_GDIColor.r=GetRValue(color);
	m_GDIColor.g=GetGValue(color);
	m_GDIColor.b=GetBValue(color);

	return old;
}

void CCanvas::Polyline(const POINT* pShape, int iCount)
{	
	if(!pShape||iCount<2)
		return;	
	
	m_minPt.x = 0;
	m_minPt.y = 0;
	m_maxPt.x = m_nWidth;
	m_maxPt.y = m_nHeight;
	int tc = RectClipLine((POINT*)pShape, iCount, m_pPoints, m_lpLineList, &m_minPt, &m_maxPt );
	if(tc>=1)
	{
		int pos = 0;
		for ( int jj=0; jj<tc; jj++ )
		{
			DrawLines(m_GDIHandle,m_GDIPen,(GDIPoint*)m_pPoints+ pos,m_lpLineList[jj]-pos);		
			pos = m_lpLineList[jj];
		}
	}
}

void CCanvas::DrawArrow(const POINT* pShape, int iCount, GDIColor fillcolor, int wd)
{	
	if(!pShape||iCount<2)
		return;

	::DrawArrow(m_GDIHandle,m_GDIPen,fillcolor,wd, 20, (GDIPoint*)pShape,iCount);
}

void CCanvas::PolyGradientline(const POINT* pShape,int iCount,COLORREF c)
{
	if(!pShape||iCount<2)
		return;

	m_color.a=0xff;
	m_color.r=GetRValue(c);
	m_color.g=GetGValue(c);
	m_color.b=GetBValue(c);

	m_minPt.x = -10;
	m_minPt.y = -10;
	m_maxPt.x = m_nWidth+10;
	m_maxPt.y = m_nHeight+10;
	int tc = RectClipLine((POINT*)pShape, iCount, m_pPoints, m_lpLineList, &m_minPt, &m_maxPt );
	if(tc>=1)
	{
		int pos = 0;
		int jj;
		for(jj=0; jj<tc; jj++)
		{
			DrawGradientLines(m_GDIHandle,m_GDIPen,m_color,(GDIPoint*)m_pPoints+ pos,m_lpLineList[jj]-pos,ACROSS_CENTER_GRADIENT);
			pos = m_lpLineList[jj];
		}
	}
}

void CCanvas::Polygon(const POINT* pShape, int iCount,HBITMAP  bmp)
{
	m_newpen = m_GDIPen;
	m_newpen.endCap=Flat;
	m_newpen.startCap=Flat;
	m_newpen.lineJoin=Miter;

	if(m_newpen.width>0)
	{
		m_minPt.x = -10;
		m_minPt.y = -10;
		m_maxPt.x = m_nWidth+10;
		m_maxPt.y = m_nHeight+10;
		int tc = RectClipLine((POINT*)pShape, iCount, m_pPoints, m_lpLineList, &m_minPt, &m_maxPt );
		if(tc>=1)
		{
			int pos = 0;
			for ( int jj=0; jj<tc; jj++ )
			{
				::DrawLines(m_GDIHandle, m_newpen, (GDIPoint*)m_pPoints+ pos, m_lpLineList[jj]-pos);
				pos = m_lpLineList[jj];
			}
		}
	}

	m_rcClip.left  = -10;
	m_rcClip.top   = -10;
	m_rcClip.right  = m_nWidth + 10;
	m_rcClip.bottom = m_nHeight + 10;
	int nCount = clipGraph(&m_rcClip, (POINT*)pShape, iCount, m_pPoints, FALSE);
	if ( nCount > 2 )
	{
		::FillPolygon(m_GDIHandle, m_GDIColor,(GDIPoint*)m_pPoints,nCount);
	}
}

void CCanvas::OffsetLine(const POINT* pShape, int iCount, int offset)
{
	if(!pShape||iCount<2)
		return;

	int flag = 0;
	int width = abs(2*offset);
	if (offset == 0)
	{
		Polyline(pShape, iCount);
	}
	else
	{
		if (offset>0)
			flag = 1;
		else
			flag = -1;
		::DrawLinesBorder(m_GDIHandle,m_GDIPen.color, width, m_GDIPen.width, (GDIPoint*)pShape, iCount, flag);
	}
}

//Dynamic Traffic Arrow
void CCanvas::RoadSidesArrow(const POINT* pShape, int iCount, int flag)
{
	if(!pShape||iCount<2)
		return;

	LineSideArrow(m_GDIHandle, m_GDIPen.width, 4, m_GDIPen.color, (GDIPoint*)pShape, iCount, flag);	
}

void CCanvas::Rectangle(GDIPen pen, GDIRect rect)
{	
	::DrawRectangle(m_GDIHandle, pen, rect);	
}

void CCanvas::Ellipse(GDIPen pen, GDIRect rect)
{	
	::DrawEllipse1(m_GDIHandle, pen, (rect.right+rect.left)/2, (rect.bottom+ rect.top)/2, (rect.right-rect.left)/2, (rect.bottom- rect.top)/2);
}

HFONT  CCanvas::SetGdiFont(HFONT  font)
{
	return (HFONT)SelectObject(m_hMemDC,font);
}

void CCanvas::DrawText(LPCTSTR lpString, int iCount, LPRECT lpRect, UINT uFormat)
{
	::DrawText(m_hMemDC,lpString,iCount, lpRect, uFormat);
	//m_bFlush=TRUE;
}
void CCanvas::DrawBack(COLORREF color, LPRECT rcBound)
{	
	HBRUSH hbrush = CreateSolidBrush(color);
	HPEN hPen = CreatePen( PS_NULL, 0, 0 );

	HBRUSH holdbrush = (HBRUSH)SelectObject(m_hMemDC, hbrush);
	HPEN holdPen = (HPEN)SelectObject(m_hMemDC, hPen);

	::Rectangle(m_hMemDC,rcBound->left, rcBound->top, rcBound->right, rcBound->bottom);
	::SelectObject(m_hMemDC, holdbrush);
	::SelectObject(m_hMemDC, holdPen);
	::DeleteObject(hbrush);
	::DeleteObject(hPen);
}

void CCanvas::SetScreenDC(HDC hDC)
{	
	m_hMemDC=hDC;	
}

HDC CCanvas::GetDC() const
{
	return m_hMemDC;
}

void CCanvas::Arrows(const POINT* pShape, int iCount, int wd, int len1, int len2)
{
	if(!pShape||iCount<2)
		return;

	DrawArrows(m_GDIHandle, m_GDIPen.color, m_GDIColor, wd, len1, len2, (GDIPoint*)pShape, iCount);
}

void CCanvas::DashedLine(const POINT* pShape, int iCount, int segline_len1, int interval_len1)
{
	if (!pShape || iCount<2)
		return;

	//	::DrawDashedLines(m_GDIHandle, m_GDIPen.color, (GDIPoint*)pShape, iCount, m_GDIPen.width, segline_len1, interval_len1);

}

void CCanvas::DrawEdgeText(TCHAR* wsName, int len, RECT* rc, COLORREF Fontclr, COLORREF EdgeClr ,int align)
{
	if(len < 1)
	{
		return;
	}
	TCHAR* szName = new TCHAR[len+1];
	wcsncpy (szName, wsName, len);
	szName[len] = L'\0';
	RECT r = *rc;
#if 0
	::SetBkMode(m_hMemDC,   TRANSPARENT);  
	::BeginPath(m_hMemDC);  
	::TextOut(m_hMemDC,  
		rc->left,    
		rc->top,  
		szName,  
		len);  
	::EndPath(m_hMemDC);  

	int w = 2;
	HPEN hPen = ::CreatePen(PS_SOLID,w, EdgeClr);
	HBRUSH hBrush = ::CreateSolidBrush(Fontclr);
	HGDIOBJ oldPen =  ::SelectObject(m_hMemDC, hPen);
	HGDIOBJ oldBrush =  ::SelectObject(m_hMemDC, hBrush);
	::StrokeAndFillPath(m_hMemDC);
	::SelectObject(m_hMemDC, oldPen);
	::SelectObject(m_hMemDC, oldBrush);

	COLORREF clr = SetTextColor(m_hMemDC, Fontclr);
	::TextOut(m_hMemDC,  
		rc->left+w/2,    
		rc->top+w/2,  
		szName,  
		len);  
	delete []szName;
	SetTextColor(m_hMemDC, clr);
#else
	COLORREF clr = SetTextColor(m_hMemDC, EdgeClr);
	r.top += 1;
	r.bottom += 1;
	::DrawText(m_hMemDC, szName, _tcslen(szName), &r, align);
	r.top -= 2;
	r.bottom -= 2;
	::DrawText(m_hMemDC, szName, _tcslen(szName), &r, align);
	r.top += 1;
	r.bottom += 1;
	r.left += 1;
	r.right += 1;
	::DrawText(m_hMemDC, szName, _tcslen(szName), &r, align);
	r.left -= 2;
	r.right -= 2;
	::DrawText(m_hMemDC, szName, _tcslen(szName), &r, align);
	r.left += 1;
	r.right += 1;
	SetTextColor(m_hMemDC, Fontclr);
	::DrawText(m_hMemDC, szName, _tcslen(szName), &r, align);
	SetTextColor(m_hMemDC, clr);
	delete []szName;
#endif
}
void CCanvas::DrawText(TCHAR* wsName, RECT* rc, COLORREF Fontclr)
{

	COLORREF clr = SetTextColor(m_hMemDC, Fontclr);
	::DrawText(m_hMemDC, wsName, _tcslen(wsName), rc, DT_LEFT|DT_TOP);
	SetTextColor(m_hMemDC, clr);
}

void CCanvas::SetDirArrowAttribute(DirArrowAttribute stDirArrowAttri)
{
	m_dirArrowAttri = stDirArrowAttri;
}

//gan 路口放大图的提示箭头 2008.09.05
void CCanvas::DrawDirecttionArrow(POINT referPt, double dDirection)
{
	if(m_dirArrowAttri.headLength<1 || m_dirArrowAttri.headWidth<1 || m_dirArrowAttri.tailLength<1 
		|| m_dirArrowAttri.tailWidth1<1 || m_dirArrowAttri.tailWidth2<1)
		return;

	double angle = dDirection / 57.295779513;
	POINT pts[3];
	POINT point1, point2;
	double m_a,m_b,m_c;
	double m_a1,m_b1,m_c1;
	int wd = 0;

	POINT ptsDraw[8];
	POINT headPTS[3];
	POINT tailPTS[4];
	int headLength = m_dirArrowAttri.headLength;
	int headWidth = m_dirArrowAttri.headWidth;
	int tailLenth = m_dirArrowAttri.tailLength;
	int tailWidth1 = m_dirArrowAttri.tailWidth1;
	int tailWidth2 = m_dirArrowAttri.tailWidth2;

	for (int i = 0; i<2; i++)
	{
		if (i==0)
		{
			point1 = referPt;
			point2.x = point1.x + headLength*cos(angle);
			point2.y = point1.y - headLength*sin(angle);
			wd = headWidth;
		}
		else
		{
			point2 = referPt;
			point1.x = point2.x - tailLenth*cos(angle);
			point1.y = point2.y + tailLenth*sin(angle);
			wd = tailWidth2;
		}
		//－－－－－－
		m_a=point1.y-point2.y;
		m_b=point2.x-point1.x;

		//水平方向
		if(fabs(m_a)<=5)
		{
			POINT tempt={0, 0};
			tempt.x = point1.x;
			tempt.y = point1.y;

			pts[0].x=tempt.x;
			pts[0].y=(int)(tempt.y-wd/2);
			pts[1].x = tempt.x;
			pts[1].y = (int)(tempt.y + wd/2);
			pts[2] = point2;
		}
		//垂直方向
		else if(fabs(m_b)<=5)
		{
			POINT tempt = {0, 0};
			tempt.x = point1.x;
			tempt.y = point1.y;
			pts[0].x=(int)(tempt.x+wd/2);
			pts[0].y=tempt.y;
			pts[1].x=(int)(tempt.x-wd/2);
			pts[1].y=tempt.y;
			pts[2] = point2;
		}
		else
		{	
			POINT tempt1 = point1;
			POINT tempt2 = point2;
			tempt1.x = point1.x;
			tempt1.y = point1.y;
			tempt2.x = point2.x;
			tempt2.y = point2.y;

			m_a=tempt2.y-tempt1.y;
			m_b=tempt1.x-tempt2.x;	

			m_c=tempt2.x*tempt1.y-tempt1.x*tempt2.y;
			m_a1=tempt2.x-tempt1.x;	
			m_b1=tempt2.y-tempt1.y;
			m_c1=m_b*tempt1.x-m_a*tempt1.y;

			pts[2] = point2;

			pts[0].x = -wd*0.5*(point2.y - point1.y)/(DISTANCE(point1, point2)) + point1.x;
			pts[0].y = wd*0.5*(point2.x - point1.x)/(DISTANCE(point1, point2)) + point1.y;
			pts[1].x = wd*0.5*(point2.y - point1.y)/(DISTANCE(point1, point2)) + point1.x;
			pts[1].y = -wd*0.5*(point2.x - point1.x)/(DISTANCE(point1, point2)) + point1.y;
		}
		//－－－－－－－
		if (i==0)
		{
			headPTS[0] = pts[0];
			headPTS[1] = pts[1];
			headPTS[2] = pts[2];
		}
		else
		{
			tailPTS[1] = pts[0];
			tailPTS[2] = pts[1];
		}
	}

	float k1 = (float)(headWidth-tailWidth1)/(2*headWidth);
	tailPTS[0].x = headPTS[0].x + k1*(headPTS[1].x - headPTS[0].x);
	tailPTS[0].y = headPTS[0].y + k1*(headPTS[1].y - headPTS[0].y);
	tailPTS[3].x = headPTS[1].x + k1*(headPTS[0].x - headPTS[1].x);
	tailPTS[3].y = headPTS[1].y + k1*(headPTS[0].y - headPTS[1].y);

	ptsDraw[0] = headPTS[2];
	ptsDraw[1] = headPTS[0];
	ptsDraw[2] = tailPTS[0];
	ptsDraw[3] = tailPTS[1];
	ptsDraw[4] = tailPTS[2];
	ptsDraw[5] = tailPTS[3];
	ptsDraw[6] = headPTS[1];
	ptsDraw[7] = headPTS[2];
	::FillPolygon(m_GDIHandle,m_GDIColor,(GDIPoint*)ptsDraw,8);
	Polyline(ptsDraw, 8);
}