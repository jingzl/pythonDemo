#pragma once
#include "IMapDisplay.h"
#include "IParser.h"
#include "IMapOutput.h"




class CDynMapTileMgr 
{
public:
    CDynMapTileMgr();
    ~CDynMapTileMgr();

    int Init( const char* path );

    int GetMapTile( int iZoomx, double Xmin, double Ymin, double Xmax, double Ymax );

protected:
    
    void OutputMessage( CString str );
    void Release();

private:

    CSysPara m_SysPara;
    CMapDisplayCfg m_MapDisplayCfg;
    CDisplayModeCfg m_DisplayModeCfg;

    // 数据解析对象
    IParser* m_lpParser;
    // 地图绘制对象
    IMapDisplay* m_lpMapDisplay;
    // 地图输出对象
    IMapOutput* m_lpMapOutput;



};
