#include "stdafx.h"
#include "DynMapTileMgr.h"


CDynMapTileMgr::CDynMapTileMgr()
{
    m_lpParser = NULL;
    m_lpMapDisplay = NULL;
    m_lpMapOutput = NULL;
}

CDynMapTileMgr::~CDynMapTileMgr()
{
    //Release();
}

int CDynMapTileMgr::Init( const char* path )
{
    CString strMsg;
    if ( !m_SysPara.Init( path ) )
    {
        strMsg.Format( _T("SysPara Init Failed !") );
        OutputMessage( strMsg );
        return -1;
    }
    if ( !m_MapDisplayCfg.Init( m_SysPara.m_strMapDisplayCfgFile.c_str() ) )
    {
        strMsg.Format( _T("Map Display Config Init Failed !") );
        OutputMessage( strMsg );
        return -1;
    }
    if ( !m_DisplayModeCfg.Init( m_SysPara.m_strDisplayModeCfgFile.c_str() ) )
    {
        strMsg.Format( _T("Display Mode Config Init Failed !") );
        OutputMessage( strMsg );
        return -1;
    }
    strMsg.Format( _T("init config ok !") );
    OutputMessage( strMsg );

    m_lpParser = GetParserInstance();
    if ( m_lpParser == NULL )
    {
        strMsg.Format( _T("地图解析对象创建失败！") );
        OutputMessage( strMsg );
        return -1;
    }
    m_lpParser->SetMapDisplayConfig( m_MapDisplayCfg.get_DisplayMode_Ary() );
    m_lpParser->SetDisplayModeConfig( &m_DisplayModeCfg.m_PointDisplayMode_ary );
    m_lpParser->SetDisplayModeConfig( &m_DisplayModeCfg.m_LineDisplayMode_ary );
    m_lpParser->SetDisplayModeConfig( &m_DisplayModeCfg.m_AreaDisplayMode_ary );
    m_lpParser->SetDisplayModeConfig( &m_DisplayModeCfg.m_TxtDisplayMode_ary );
    m_lpParser->SetMapScaleConfig( &m_SysPara.m_MapScaleAry );
    if ( !m_lpParser->Init( m_SysPara.m_strDataPath.c_str() ) )
    {
        strMsg.Format( _T("地图解析对象初始化失败！") );
        OutputMessage( strMsg );
        return -1;
    }
    m_lpMapDisplay = CreateMapDisplay();
    if ( m_lpMapDisplay == NULL )
    {
        strMsg.Format( _T("地图显示对象创建失败！") );
        OutputMessage( strMsg );
        return -1;
    }
    SIZE bufsize;
    bufsize.cx = DRAWMAP_BUFFER_WIDTH;
    bufsize.cy = DRAWMAP_BUFFER_HEIGHT;
    wstring wstr = str2wstr( m_SysPara.m_strConfigPath );
    m_lpMapDisplay->SetImagePath( wstr.c_str() );
    if ( !m_lpMapDisplay->Initialize( bufsize ) )
    {
        strMsg.Format( _T("地图显示对象初始化失败！") );
        OutputMessage( strMsg );
        return -1;
    }

    m_lpMapOutput = GetMapOutputInstance();
    if ( m_lpMapOutput == NULL )
    {
        strMsg.Format( _T("Map Grid Output Object Create Failed !") );
        OutputMessage( strMsg );
        return -1;
    }
    if ( !m_lpMapOutput->InitMapOutput( m_lpMapDisplay, m_lpParser, m_SysPara.m_strOutputMapPath.c_str(), m_SysPara.m_strOutputTxtPath.c_str() ) )
    {
        strMsg.Format( _T("Map Grid Output Object Init Failed !") );
        OutputMessage( strMsg );
        return -1;
    }

    strMsg.Format( _T("init engine ok !") );
    OutputMessage( strMsg );

    // 释放时会导致python调用异常，此处暂时屏蔽
    //Release();

    return 0;
}

int CDynMapTileMgr::GetMapTile( int iZoomx, double Xmin, double Ymin, double Xmax, double Ymax )
{
    CString strMsg;
    if ( !m_lpMapOutput->OutputMap( iZoomx, Xmin, Ymin, Xmax, Ymax, m_SysPara.m_nDisplayMode ) )
    {
        strMsg.Format( _T("Map Grid Output Failed !") );
        OutputMessage( strMsg );
        return -1;
    }
    strMsg.Format( _T("Output Map ok !") );
    OutputMessage( strMsg );
    return 0;
}


//////////////////////////////////////////////////////////////////////////
// 
void CDynMapTileMgr::OutputMessage( CString str )
{
    _tprintf( str );
    _tprintf( _T("\n") );
}

void CDynMapTileMgr::Release()
{
    if ( m_lpMapDisplay != NULL )
    {
        m_lpMapDisplay->Release();
        m_lpMapDisplay = NULL;
    }

    if ( m_lpParser != NULL )
    {
        m_lpParser->Release();
        m_lpParser = NULL;
    }

    if ( m_lpMapOutput != NULL )
    {
        m_lpMapOutput->Release();
        m_lpMapOutput = NULL;
    }
}



