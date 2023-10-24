#pragma once
#include "IMapDisplay.h"
#include "IParser.h"

#define HDC_SIZE_WEIGHT    512

// 以左上角即西北角为原点
#define MAP_MIN_X           -180
#define MAP_MAX_X           180
#define MAP_MIN_Y           -85.05112878
#define MAP_MAX_Y           85.05112878



class IMapOutput 
{
public:

    // 设置日志
    virtual void SetLogFunc( LogFunc lplogfunc ) = 0;

	//初始化
	virtual BOOL InitMapOutput( IMapDisplay* pMapDisplay, IParser* pParser, const char* lpszPicPath, const char* lpszTxtPath ) = 0;

	//地图输出
	virtual BOOL OutputMap( int iZoomx, double Xmin, double Ymin, double Xmax, double Ymax, BYTE byteBigDisplayMode=0 ) = 0;

    // 释放
    virtual void Release() = 0;

};

IMapOutput* GetMapOutputInstance();
