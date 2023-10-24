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

	//��������: ���ų�ʼ��
	//�������: ��; 
	//�������: ��
	//���ؽ��: ��
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

	//��������: ���ų�ʼ��
	//�������: ��; 
	//�������: ��
	//���ؽ��: ��
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
	//��CGeoRender�������г�Ա
	friend class CGeoRender;

	CGeoPolygonStyle();
	virtual ~CGeoPolygonStyle();
	void InitMe(st_areadisplaymode* pStyle);

	//��������: ���ų�ʼ��
	//�������: ��; 
	//�������: ��
	//���ؽ��: ��
	void InitStyle(CCanvas* pCanvas);

	void UnInitStyle(CCanvas* pCanvas);
private:
	//�߽��

	GDIPen				m_EdgePen;

	GDIPen				m_oldpen;
	COLORREF			m_OldClor;	
};

class CGeoPolylineStyle : public st_linedisplaymode
{
public:
	//��CGeoRender�������г�Ա
	friend class CGeoRender;
	CGeoPolylineStyle();
	virtual ~CGeoPolylineStyle();

	void InitMe(st_linedisplaymode* pStyle);
	void InitMeBack(st_linedisplaymode* pStyle);

	//��������: ���ų�ʼ��
	//�������: ��; 
	//�������: ��
	//���ؽ��: ��
	void InitStyle(CCanvas* pCanvas);
	void UnInitStyle(CCanvas* pCanvas);

private:
	//���Ƶĳ���1
	unsigned short		m_Length1;
	//���Ƶĳ���2
	unsigned short		m_Length2;
	//
	unsigned short		m_EdgeWidth;
	//����ǰ�εı�
	GDIPen			m_FillPen1;
	//������һ�εı�
	GDIPen			m_FillPen2;

	//����ǰ�α߽�ı�
	GDIPen			m_EdgePen1;

	//�����¶α߽�ı�
	GDIPen			m_EdgePen2;

	GDIPen			m_oldpen;

	//ʣ�೤�ȣ�����ϴλ���
	int				m_Lastdis;
	BOOL            m_bAlter;
};

//�������
class CGeoRender  
{
public:
	CGeoRender();
	virtual ~CGeoRender();
	

	//��������: �������ƶ����
	//�������: pCanvas, ��ʾ����; 
	//			pPnts,Ҫ��ʾ����Ļ������; 
	//			nCount,��������Ŀ;
	//			pStyle,���������
	//�������: ��
	//���ؽ��: ��
	static void Polygon(CCanvas* pCanvas,	
		const POINT* pPnts, int nCount, CGeoPolygonStyle& style);//���ƶ��

	static void Polyline(CCanvas* pCanvas,
		const POINT* pPnts, int nCount, CGeoPolylineStyle& style);

	static void DrawPoint(CCanvas* pCanvas,
		const POINT* pPnt, CGeoPointStyle* style);
protected:
	static int LENGTH(int DX, int DY);
	//��������: �����������
	//�������: pCanvas, ��ʾ����; 
	//			pPnts,Ҫ��ʾ����Ļ������; 
	//			nCount,��������Ŀ;
	//			pStyle,���������
	//�������: ��
	//���ؽ��: ��
	static void DrawAlternatingLine(CCanvas* pCanvas,	
		const POINT* pPnts, int& nCount, 
		CGeoPolylineStyle* pStyle );//����·��



};