#pragma once
#include "globaldefine.h"



class IParser
{
public:
    // ������־
    virtual void SetLogFunc( LogFunc lplogfunc ) = 0;

    // ���õ�ͼ��ʾ����
    virtual void SetMapDisplayConfig( const ST_DISPLAYMODE_ARY* lpDisplayModeAry ) = 0;

    // ������ʾ��ʽ����
    virtual void SetDisplayModeConfig( const ST_POINTDISPLAYMODE_ARY* lpPointDisplayModeAry ) = 0;
    virtual void SetDisplayModeConfig( const ST_LINEDISPLAYMODE_ARY* lpLineDisplayModeAry ) = 0;
    virtual void SetDisplayModeConfig( const ST_AREADISPLAYMODE_ARY* lpAreaDisplayModeAry ) = 0;
    virtual void SetDisplayModeConfig( const ST_TXTDISPLAYMODE_ARY* lpTxtDisplayModeAry ) = 0;

    // ���õ�ǰ�ĵ�ͼ�����ߵȼ�����
    virtual void SetMapScaleConfig( ST_SCALECFG_ARY* scalecfgary ) = 0;

    // Init
    virtual BOOL Init( const char* lpszDataPath ) = 0;

    // ��ȡ����
    virtual BOOL GetMapData( int nDisplayMode, BYTE nMapScale, GEORECT rect, PMAP_DISP_DATA& lpMapDispData ) = 0;

    // �ͷŵ�ͼ����
    virtual void ReleaseMapData( PMAP_DISP_DATA lpMapDispData ) = 0;

    // �ͷ�
    virtual void Release() = 0;
};

IParser* GetParserInstance();
