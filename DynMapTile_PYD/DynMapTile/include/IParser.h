#pragma once
#include "globaldefine.h"



class IParser
{
public:
    // 设置日志
    virtual void SetLogFunc( LogFunc lplogfunc ) = 0;

    // 设置地图显示配置
    virtual void SetMapDisplayConfig( const ST_DISPLAYMODE_ARY* lpDisplayModeAry ) = 0;

    // 设置显示样式配置
    virtual void SetDisplayModeConfig( const ST_POINTDISPLAYMODE_ARY* lpPointDisplayModeAry ) = 0;
    virtual void SetDisplayModeConfig( const ST_LINEDISPLAYMODE_ARY* lpLineDisplayModeAry ) = 0;
    virtual void SetDisplayModeConfig( const ST_AREADISPLAYMODE_ARY* lpAreaDisplayModeAry ) = 0;
    virtual void SetDisplayModeConfig( const ST_TXTDISPLAYMODE_ARY* lpTxtDisplayModeAry ) = 0;

    // 设置当前的地图比例尺等级配置
    virtual void SetMapScaleConfig( ST_SCALECFG_ARY* scalecfgary ) = 0;

    // Init
    virtual BOOL Init( const char* lpszDataPath ) = 0;

    // 获取数据
    virtual BOOL GetMapData( int nDisplayMode, BYTE nMapScale, GEORECT rect, PMAP_DISP_DATA& lpMapDispData ) = 0;

    // 释放地图数据
    virtual void ReleaseMapData( PMAP_DISP_DATA lpMapDispData ) = 0;

    // 释放
    virtual void Release() = 0;
};

IParser* GetParserInstance();
