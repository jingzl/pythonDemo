#include "StdAfx.h"
#include "SysPara.h"


CSysPara::CSysPara(void)
{
    m_nDisplayMode = 0;
    m_dMinX = 180;
    m_dMinY = 90;
    m_dMaxX = -180;
    m_dMaxY = -90;

    /*
	TCHAR exeFullPath[MAX_PATH];
	GetModuleFileName( NULL, exeFullPath, MAX_PATH );
	m_strAppPath.Format( _T("%s"), exeFullPath );
	int nFind = m_strAppPath.ReverseFind( '\\' );
	m_strAppPath = m_strAppPath.Left( nFind + 1 );

    m_strLogPath = m_strAppPath + _T("log");
    */
}

CSysPara::~CSysPara(void)
{
    Release();
}

BOOL CSysPara::Init( const char* lpszPath )
{
    Release();

    m_strAppPath = lpszPath;
    m_strLogPath = m_strAppPath;
    m_strLogPath.insert( m_strLogPath.size(), "\\log" );

    m_strConfigPath = m_strAppPath;
    string strCfgFile;
	strCfgFile = m_strAppPath;
    strCfgFile.insert( strCfgFile.size(), SYS_CONFIGFILE_PATH );
    strCfgFile.insert( strCfgFile.size(), SYS_CONFIGFILE_NAME );
    
	TiXmlDocument xmlDoc( strCfgFile.c_str() );
    printf( strCfgFile.c_str() );
    printf( "\n" );
	if ( !xmlDoc.LoadFile() )
	{
		return FALSE;
	}
	TiXmlElement* pRootElem = NULL;
	TiXmlElement* pElem = NULL;
    TiXmlElement* pChildElem = NULL;

	pRootElem = xmlDoc.FirstChildElement( "MapMaster_Config" );
	if ( pRootElem == NULL )
	{
		return FALSE;
	}

	// data_path
	pElem = pRootElem->FirstChildElement( "data_path" );
	if ( pElem == NULL )
	{
		return FALSE;
	}
    if ( pElem->FirstChild()->Value() != NULL )
    {
        m_strDataPath = pElem->FirstChild()->Value();
    }

    pElem = pRootElem->FirstChildElement( "outmap_path" );
    if ( pElem == NULL )
    {
        return FALSE;
    }
    if ( pElem->FirstChild()->Value() != NULL )
    {
        m_strOutputMapPath = pElem->FirstChild()->Value();
    }

    pElem = pRootElem->FirstChildElement( "outtxt_path" );
    if ( pElem == NULL )
    {
        return FALSE;
    }
    if ( pElem->FirstChild()->Value() != NULL )
    {
        m_strOutputTxtPath = pElem->FirstChild()->Value();
    }

    // display mode
    pElem = pRootElem->FirstChildElement( "displaymode" );
    if ( pElem == NULL )
    {
        return FALSE;
    }
    if ( pElem->FirstChild()->Value() != NULL )
    {
        m_nDisplayMode = atoi( pElem->FirstChild()->Value() );
    }

    // map rect
    pElem = pRootElem->FirstChildElement( "maprect" );
    if ( pElem == NULL )
    {
        return FALSE;
    }
    if ( pElem->Attribute("dminx") != NULL )
    {
        m_dMinX = atof( pElem->Attribute("dminx") );
    }
    if ( pElem->Attribute("dminy") != NULL )
    {
        m_dMinY = atof( pElem->Attribute("dminy") );
    }
    if ( pElem->Attribute("dmaxx") != NULL )
    {
        m_dMaxX = atof( pElem->Attribute("dmaxx") );
    }
    if ( pElem->Attribute("dmaxy") != NULL )
    {
        m_dMaxY = atof( pElem->Attribute( "dmaxy" ) );
    }

    // map scale
    pElem = pRootElem->FirstChildElement("mapscale");
    if ( pElem == NULL )
    {
        return FALSE;
    }
    int nScaleNum = atoi( pElem->Attribute("scale_num") );
    if ( nScaleNum > 0 )
    {
        pChildElem = pElem->FirstChildElement( "scale" );
        if ( pElem == NULL )
        {
            return FALSE;
        }
        while ( pChildElem != NULL )
        {
            LPST_SCALECFG lpScaleCfg = new ST_SCALECFG();
            lpScaleCfg->m_nScale = atoi( pChildElem->FirstChild()->Value() );
            lpScaleCfg->m_nMLevel = TransMLevel( pChildElem->Attribute("mlevel") );
            m_MapScaleAry.push_back( lpScaleCfg );

            pChildElem = pChildElem->NextSiblingElement();
        }
    }
	// config_file
	pElem = pRootElem->FirstChildElement( "config_file" );
	if ( pElem == NULL )
	{
		return FALSE;
	}

    pChildElem = pElem->FirstChildElement( "mapdisplay" );
    if ( pChildElem == NULL )
    {
        return FALSE;
    }
    string str = pChildElem->FirstChild()->Value();
    m_strMapDisplayCfgFile = m_strAppPath;
    m_strMapDisplayCfgFile.insert( m_strMapDisplayCfgFile.size(), SYS_CONFIGFILE_PATH );
    m_strMapDisplayCfgFile.insert( m_strMapDisplayCfgFile.size(), ("\\") );
    m_strMapDisplayCfgFile.insert( m_strMapDisplayCfgFile.size(), str );

    pChildElem = pElem->FirstChildElement( "dispmode" );
    if ( pChildElem == NULL )
    {
        return FALSE;
    }
    str = pChildElem->FirstChild()->Value();
    m_strDisplayModeCfgFile = m_strAppPath;
    m_strDisplayModeCfgFile.insert( m_strDisplayModeCfgFile.size(), SYS_CONFIGFILE_PATH );
    m_strDisplayModeCfgFile.insert( m_strDisplayModeCfgFile.size(), ("\\") );
    m_strDisplayModeCfgFile.insert( m_strDisplayModeCfgFile.size(), str );

	return TRUE;
}

void CSysPara::Release()
{
    for ( ST_SCALECFG_ARY::iterator it=m_MapScaleAry.begin(); it!=m_MapScaleAry.end(); it++ )
    {
        LPST_SCALECFG lpScaleCfg = *it;
        if ( lpScaleCfg != NULL )
        {
            delete lpScaleCfg;
            lpScaleCfg = NULL;
        }
    }
    m_MapScaleAry.clear();
}

int CSysPara::TransMLevel( const char* level )
{
    int nIdx = 1;
    if ( strcmp( level, "g" ) == 0 )
    {
        nIdx = 0;
    }
    else if ( strcmp( level, "i" ) == 0 )
    {
        nIdx = 1;
    }
    else if ( strcmp( level, "j" ) == 0 )
    {
        nIdx = 2;
    }
    else if ( strcmp( level, "k" ) == 0 )
    {
        nIdx = 3;
    }
    else if ( strcmp( level, "l" ) == 0 )
    {
        nIdx = 4;
    }
    else if ( strcmp( level, "m" ) == 0 )
    {
        nIdx = 5;
    }
    else if ( strcmp( level, "n" ) == 0 )
    {
        nIdx = 6;
    }
    else if ( strcmp( level, "o" ) == 0 )
    {
        nIdx = 7;
    }
    else if ( strcmp( level, "p" ) == 0 )
    {
        nIdx = 8;
    }
    else if ( strcmp( level, "q" ) == 0 )
    {
        nIdx = 9;
    }
    else
    {
        nIdx = 1;
    }
    return nIdx;
}

