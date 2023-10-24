#pragma once

#include "Gdiplus2.h"
#ifdef _DEBUG
#pragma comment ( lib, "gdi2_d.lib" )
#else
#pragma comment ( lib, "gdi2.lib" )
#endif

typedef struct _tagDirArrowAttri 
{
	int headWidth;
	int headLength;
	int tailLength;
	int tailWidth1;
	int tailWidth2;
}DirArrowAttribute;

class CCanvas  
{
public:
	CCanvas();
	~CCanvas();	
	BOOL Create(int iWidth, int iHeight);
	void Release();
	BOOL Blt(CCanvas *pSrcsurface, LPRECT pDestRect, LPRECT pSrcRect, DWORD dwRop = SRCCOPY);
	void SetScreenDC(HDC hDC);	
	void Refresh(HDC hdc, int iOffsetX = 0, int iOffsetY = 0, LPRECT lpRect = NULL);	
	GDIPen   SetGdiPen(GDIPen* pen);
	COLORREF SetGdiBrushColor(COLORREF color);
	HFONT    SetGdiFont(HFONT  font);
	void Polyline(const POINT* pShape, int iCount);
	void PolyGradientline(const POINT* pShape,int iCount,COLORREF c);
	void OffsetLine(const POINT* pShape, int iCount, int offset);
	void RoadSidesArrow(const POINT* pShape, int iCount, int flag);
	void Arrows(const POINT* pShape, int iCount, int wd, int len1, int len2);
	void DashedLine(const POINT* pShape, int iCount, int segline_len1, int interval_len1);	
	void DrawArrow(const POINT* pShape, int iCount, GDIColor fillcolor, int wd=20);
	void Polygon(const POINT* pShape, int iCount,HBITMAP  bmp=NULL);
	void Rectangle(GDIPen pen, GDIRect rect);
	void Ellipse(GDIPen pen, GDIRect rect);
	void DrawText(LPCTSTR lpString, int iCount, LPRECT lpRect, UINT uFormat = DT_WORDBREAK);
	void DrawBack(COLORREF color, LPRECT rcBound);
	void Flush();
	HDC GetDC() const;
	void DrawEdgeText(TCHAR* wsName, int len, RECT* rc, COLORREF Fontclr, COLORREF EdgeClr , int align=DT_WORDBREAK);
	void DrawText(TCHAR* wsName, RECT* rc, COLORREF Fontclr);
	GDIHandle GetGDIHandle() { return m_GDIHandle; };

	//路口放大图的提示箭头 2008.09.05 
	/*
	* @param [in]		referPt 参考点坐标
	* @param [in]		dDirection  提示方向
	* @return 
	*/
	void DrawDirecttionArrow(POINT referPt, double dDirection);
	void SetDirArrowAttribute(DirArrowAttribute stDirArrowAttri);

private:	
	GDIHandle     m_GDIHandle;
	BOOL		  m_bCreated;
	GDIPen        m_GDIPen;
	GDIColor      m_GDIColor;

	int				m_nWidth;
	int				m_nHeight;

	HDC             m_hMemDC;

	DirArrowAttribute   m_dirArrowAttri;
	POINT*          m_pPoints;


    POINT m_minPt;
    POINT m_maxPt;
    int* m_lpLineList;
    GDIColor m_color;
    GDIPen m_newpen;
    RECT m_rcClip;

};