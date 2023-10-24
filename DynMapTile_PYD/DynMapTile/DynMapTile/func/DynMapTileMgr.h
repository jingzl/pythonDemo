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

    // ���ݽ�������
    IParser* m_lpParser;
    // ��ͼ���ƶ���
    IMapDisplay* m_lpMapDisplay;
    // ��ͼ�������
    IMapOutput* m_lpMapOutput;



};
