#pragma once
#include "globaldefine.h"


enum _tag_map_pos
{
	MP_CENTER = 0,  // ���ĵ�
	MP_LEFT_TOP,	// ���Ͻ�
	MP_LEFT_BOTTOM, // ���½�
};

class IMapDisplay 
{
public:
	// ��ʼ��
	virtual BOOL Initialize(SIZE bufSize) = 0;
	// ����ͼƬ·��
	virtual void SetImagePath( const TCHAR* lpszPath) = 0;
	// ���õ�ͼλ��
	virtual BOOL SetMapPos(GEOPOINT pos, _tag_map_pos mapPos = MP_CENTER ) = 0;
	// ���ñ�����
	virtual BOOL SetMapScale(double dfScale, int scale) = 0;
	// �ƶ�
	virtual BOOL MoveMap(double dfOffsetX, double dfOffsetY) = 0;
    // ��þ�γ��
    virtual BOOL GetLLPos( POINT& pt, GEOPOINT& geopt, int& nCol, int& nRow ) = 0;
	// ��ȡ��ͼ��Χ
	virtual GEORECT GetMapRect() = 0;
    // ��ȡ���ĵ�
    virtual GEOPOINT GetCenterPos() = 0;
	// ��ͼ
	virtual BOOL DrawMap( DWORD dwBackColor, PMAP_DISP_DATA pData, HDC* pDC, PTILETEXT_INFO pTileTextInfo=NULL, BOOL bGridLine=FALSE ) = 0;
    // �ͷŶ���
	virtual void Release() = 0;
};

IMapDisplay* CreateMapDisplay();
int GetBinPath(TCHAR* path);
