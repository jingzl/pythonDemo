#pragma once
#include "globaldefine.h"
#include "Canvas.h"
#include <atlimage.h>

class CGeoPointStyle: public st_pointdisplaymode
{
public:
	friend class CGeoRender;

	CGeoPointStyle();
	virtual ~CGeoPointStyle();
	void InitMe(st_pointdisplaymode* pStyle);

	//函数功能: 符号初始化
	//输入参数: 无; 
	//输出参数: 无
	//返回结果: 无
	void InitStyle(CCanvas* pCanvas);

	void UnInitStyle(CCanvas* pCanvas);

	void SetImagePath(TCHAR* lpszPath);

	void SetTransparentColor(COLORREF clr);
protected:
	void SetTransparentColor();
	CPngImg* GetImage();
	TCHAR    m_szPath[MAX_PATH];
	HFONT    m_hTrueTypeFont;
	COLORREF m_crTextOld;
	COLORREF m_TransparentColor;
	CPngImg*  m_Image;
	TCHAR    m_szImageFile[MAX_PATH];
};

class CGeoTextStyle: public st_txtdisplaymode
{
public:
	friend class CGeoRender;

	CGeoTextStyle();
	virtual ~CGeoTextStyle();
	void InitMe(st_txtdisplaymode* pStyle);

	//函数功能: 符号初始化
	//输入参数: 无; 
	//输出参数: 无
	//返回结果: 无
	void InitStyle(CCanvas* pCanvas);
	void UnInitStyle(CCanvas* pCanvas);

public:
    GDIPen m_FillPen1;
    GDIPen m_FillPen2;

private:
	COLORREF m_crold;
	COLORREF m_crBkOld;
	int m_ioldMode;
	HFONT m_holdfont;
	HFONT m_Font;

    GDIPen m_oldpen;

};

class CGeoPolygonStyle : public st_areadisplaymode
{
public:
	//让CGeoRender访问所有成员
	friend class CGeoRender;

	CGeoPolygonStyle();
	virtual ~CGeoPolygonStyle();
	void InitMe(st_areadisplaymode* pStyle);

	//函数功能: 符号初始化
	//输入参数: 无; 
	//输出参数: 无
	//返回结果: 无
	void InitStyle(CCanvas* pCanvas);

	void UnInitStyle(CCanvas* pCanvas);
private:
	//边界笔

	GDIPen				m_EdgePen;

	GDIPen				m_oldpen;
	COLORREF			m_OldClor;	
};

class CGeoPolylineStyle : public st_linedisplaymode
{
public:
	//让CGeoRender访问所有成员
	friend class CGeoRender;
	CGeoPolylineStyle();
	virtual ~CGeoPolylineStyle();

	void InitMe(st_linedisplaymode* pStyle);
	void InitMeBack(st_linedisplaymode* pStyle);

	//函数功能: 符号初始化
	//输入参数: 无; 
	//输出参数: 无
	//返回结果: 无
	void InitStyle(CCanvas* pCanvas);
	void UnInitStyle(CCanvas* pCanvas);

private:
	//绘制的长度1
	unsigned short		m_Length1;
	//绘制的长度2
	unsigned short		m_Length2;
	//
	unsigned short		m_EdgeWidth;
	//绘制前段的笔
	GDIPen			m_FillPen1;
	//绘制下一段的笔
	GDIPen			m_FillPen2;

	//绘制前段边界的笔
	GDIPen			m_EdgePen1;

	//绘制下段边界的笔
	GDIPen			m_EdgePen2;

	GDIPen			m_oldpen;

	//剩余长度，解决上次绘制
	int				m_Lastdis;
	BOOL            m_bAlter;
};

//物体绘制
class CGeoRender  
{
public:
	CGeoRender();
	virtual ~CGeoRender();
	

	//函数功能: 按风格绘制多边形
	//输入参数: pCanvas, 显示对象; 
	//			pPnts,要显示的屏幕点序列; 
	//			nCount,点序列数目;
	//			pStyle,风格对象参数
	//输出参数: 无
	//返回结果: 无
	static void Polygon(CCanvas* pCanvas,	
		const POINT* pPnts, int nCount, CGeoPolygonStyle& style);//绘制多边

	static void Polyline(CCanvas* pCanvas,
		const POINT* pPnts, int nCount, CGeoPolylineStyle& style);

	static void DrawPoint(CCanvas* pCanvas,
		const POINT* pPnt, CGeoPointStyle* style);
protected:
	static int LENGTH(int DX, int DY);
	//函数功能: 交替绘制线条
	//输入参数: pCanvas, 显示对象; 
	//			pPnts,要显示的屏幕点序列; 
	//			nCount,点序列数目;
	//			pStyle,风格对象参数
	//输出参数: 无
	//返回结果: 无
	static void DrawAlternatingLine(CCanvas* pCanvas,	
		const POINT* pPnts, int& nCount, 
		CGeoPolylineStyle* pStyle );//绘制路线



};