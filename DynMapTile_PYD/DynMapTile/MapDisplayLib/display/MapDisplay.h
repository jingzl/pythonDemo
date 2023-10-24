#pragma once
#include "IMapDisplay.h"
#include "DisplayInfo.h"
#include "MapRender.h"



class CMapDisplay :	public IMapDisplay
{
public:
	CMapDisplay(void);
	~CMapDisplay(void);

    // ��ʼ��
	BOOL Initialize(SIZE bufSize);	
	// ����ͼƬ·��
	void SetImagePath(const TCHAR* lpszPath);
	// ���õ�ͼλ��
	BOOL SetMapPos(GEOPOINT pos, _tag_map_pos mapPos = MP_CENTER );
	// ���ñ�����
	BOOL SetMapScale(double dfScale, int scale);
	// �ƶ�
	BOOL MoveMap(double dfOffsetX, double dfOffsetY);
    // ��þ�γ��
    BOOL GetLLPos( POINT& pt, GEOPOINT& geopt, int& nCol, int& nRow );
	// ��ȡ��ͼ��Χ
	GEORECT GetMapRect();
    // ��ȡ���ĵ�
    GEOPOINT GetCenterPos();
	// ��ͼ
	BOOL DrawMap(DWORD dwBackColor, PMAP_DISP_DATA pData, HDC* pDC, PTILETEXT_INFO pTileTextInfo=NULL, BOOL bGridLine=FALSE );
	// �ͷŶ���
	void Release();


protected:
	CDisplayInfo m_displayInfo;
	CMapRender   m_mapRender;

};
