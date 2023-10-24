#pragma once
#include "IMapDisplay.h"
#include "DisplayInfo.h"
#include "MapRender.h"



class CMapDisplay :	public IMapDisplay
{
public:
	CMapDisplay(void);
	~CMapDisplay(void);

    // 初始化
	BOOL Initialize(SIZE bufSize);	
	// 设置图片路径
	void SetImagePath(const TCHAR* lpszPath);
	// 设置地图位置
	BOOL SetMapPos(GEOPOINT pos, _tag_map_pos mapPos = MP_CENTER );
	// 设置比例尺
	BOOL SetMapScale(double dfScale, int scale);
	// 移动
	BOOL MoveMap(double dfOffsetX, double dfOffsetY);
    // 获得经纬度
    BOOL GetLLPos( POINT& pt, GEOPOINT& geopt, int& nCol, int& nRow );
	// 获取绘图范围
	GEORECT GetMapRect();
    // 获取中心点
    GEOPOINT GetCenterPos();
	// 画图
	BOOL DrawMap(DWORD dwBackColor, PMAP_DISP_DATA pData, HDC* pDC, PTILETEXT_INFO pTileTextInfo=NULL, BOOL bGridLine=FALSE );
	// 释放对象
	void Release();


protected:
	CDisplayInfo m_displayInfo;
	CMapRender   m_mapRender;

};
