#pragma once
#include "globaldefine.h"


enum _tag_map_pos
{
	MP_CENTER = 0,  // 中心点
	MP_LEFT_TOP,	// 左上角
	MP_LEFT_BOTTOM, // 左下角
};

class IMapDisplay 
{
public:
	// 初始化
	virtual BOOL Initialize(SIZE bufSize) = 0;
	// 设置图片路径
	virtual void SetImagePath( const TCHAR* lpszPath) = 0;
	// 设置地图位置
	virtual BOOL SetMapPos(GEOPOINT pos, _tag_map_pos mapPos = MP_CENTER ) = 0;
	// 设置比例尺
	virtual BOOL SetMapScale(double dfScale, int scale) = 0;
	// 移动
	virtual BOOL MoveMap(double dfOffsetX, double dfOffsetY) = 0;
    // 获得经纬度
    virtual BOOL GetLLPos( POINT& pt, GEOPOINT& geopt, int& nCol, int& nRow ) = 0;
	// 获取绘图范围
	virtual GEORECT GetMapRect() = 0;
    // 获取中心点
    virtual GEOPOINT GetCenterPos() = 0;
	// 画图
	virtual BOOL DrawMap( DWORD dwBackColor, PMAP_DISP_DATA pData, HDC* pDC, PTILETEXT_INFO pTileTextInfo=NULL, BOOL bGridLine=FALSE ) = 0;
    // 释放对象
	virtual void Release() = 0;
};

IMapDisplay* CreateMapDisplay();
int GetBinPath(TCHAR* path);
