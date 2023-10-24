#include "stdafx.h"
#include "MapDisplayCfg.h"


CMapDisplayCfg::CMapDisplayCfg()
{
}

CMapDisplayCfg::~CMapDisplayCfg()
{
    Release();
}

BOOL CMapDisplayCfg::Init( const char* strFilePath )
{
    if ( strlen(strFilePath) <= 0 )
    {
        return FALSE;
    }
    printf( strFilePath );
    printf( "\n" );
    Release();
    TiXmlDocument xmlDoc( strFilePath );
    if ( !xmlDoc.LoadFile() )
    {
        return FALSE;
    }
    TiXmlElement* pRootElem = NULL;
    TiXmlElement* pElem = NULL;
    pRootElem = xmlDoc.FirstChildElement( "mapdisplay" );
    if ( pRootElem == NULL )
    {
        return FALSE;
    }

    LPST_DISPLAYMODE lpDisplayMode = NULL;
    pElem = pRootElem->FirstChildElement( "displaystyle" );
    if ( pElem == NULL )
    {
        return FALSE;
    }
    while ( pElem != NULL )
    {
        lpDisplayMode = ImportDisplayMode( pElem );
        if ( lpDisplayMode != NULL )
        {
            m_DisplayMode_ary.push_back( lpDisplayMode );
        }
        pElem = pElem->NextSiblingElement();
    }

    return TRUE;
}

const ST_DISPLAYMODE_ARY* CMapDisplayCfg::get_DisplayMode_Ary() const
{
    return (ST_DISPLAYMODE_ARY*)&m_DisplayMode_ary;
}

const COLORREF CMapDisplayCfg::get_BackColor( BYTE nMapScaleIndex ) const
{
    std::map<BYTE,COLORREF>::const_iterator it = m_BackColor_map.find( nMapScaleIndex );
    if ( it != m_BackColor_map.end() )
    {
        return it->second;
    }
    else
    {
        return CN_BACK_COLOR;
    }
}

void CMapDisplayCfg::Release()
{
    for ( int i=0; i<m_DisplayMode_ary.size(); i++ )
    {
        LPST_DISPLAYMODE lpDisplayMode = m_DisplayMode_ary.at(i);
        if ( lpDisplayMode != NULL )
        {
            delete lpDisplayMode;
            lpDisplayMode = NULL;
        }
    }
    m_DisplayMode_ary.clear();

    m_BackColor_map.clear();
}

COLORREF CMapDisplayCfg::ExtractColor( const char* szchar )
{
    CString strClr;
    strClr = szchar;
    // eg : RGB(255,128,64);
    COLORREF clrerr = RGB(255,0,0);
    if ( strClr.GetLength() <= 0 )
    {
        // 返回红色，表示有问题
        return clrerr;
    }
    int nR = 0;
    int nG = 0;
    int nB = 0;

    int nFind = -1;
    nFind = strClr.Find( '(' );
    if ( nFind == -1 )
    {
        return clrerr;
    }
    strClr.Delete( 0, nFind+1 );
    nFind = strClr.Find( ',' );
    if ( nFind == -1 )
    {
        return clrerr;
    }
    nR = _ttol( strClr.Left(nFind) );
    strClr.Delete( 0, nFind+1 );
    nFind = strClr.Find( ',' );
    if ( nFind == -1 )
    {
        return clrerr;
    }
    nG = _ttol( strClr.Left(nFind) );
    strClr.Delete( 0, nFind+1 );
    nFind = strClr.Find( ')' );
    if ( nFind == -1 )
    {
        return clrerr;
    }
    nB = _ttol( strClr.Left(nFind) );

    COLORREF clr = RGB( nR, nG, nB );
    return clr;
}

LPST_DISPLAYMODE CMapDisplayCfg::ImportDisplayMode( TiXmlElement* pElem )
{
    if ( pElem == NULL )
    {
        return NULL;
    }
    LPST_DISPLAYMODE lpDisplayMode = NULL;
    lpDisplayMode = new ST_DISPLAYMODE();
    lpDisplayMode->m_nDisplayModeType = atoi( pElem->Attribute("type") );

    TiXmlElement* pElmChild = NULL;
    pElmChild = pElem->FirstChildElement( "scalelevel" );
    if ( pElmChild == NULL )
    {
        delete lpDisplayMode;
        lpDisplayMode = NULL;
        return NULL;
    }
    while( pElmChild != NULL )
    {
        LPST_SCALELEVEL lpScaleLevel = NULL;
        lpScaleLevel = ImportScaleLevel( pElmChild );
        if ( lpScaleLevel != NULL )
        {
            lpDisplayMode->m_scalelevel_ary.push_back( lpScaleLevel );
        }
        pElmChild = pElmChild->NextSiblingElement();
    }
    return lpDisplayMode;
}

LPST_SCALELEVEL CMapDisplayCfg::ImportScaleLevel( TiXmlElement* pElem )
{
    if ( pElem == NULL )
    {
        return NULL;
    }
    LPST_SCALELEVEL lpScaleLevel = new ST_SCALELEVEL();
    lpScaleLevel->m_nScale = atoi(pElem->Attribute( "scale" ));
    TiXmlElement* pElemChild = NULL;
    TiXmlElement* pElemLayer = NULL;

    // ground & grid color
    pElemChild = pElem->FirstChildElement( "grounding" );
    if ( pElemChild == NULL )
    {
        delete lpScaleLevel;
        lpScaleLevel = NULL;
        return NULL;
    }
    lpScaleLevel->m_clrGround = ExtractColor( pElemChild->FirstChild()->Value() );

    // 建立索引
    m_BackColor_map[lpScaleLevel->m_nScale] = lpScaleLevel->m_clrGround;

    pElemChild = pElem->FirstChildElement( "grid" );
    if ( pElemChild == NULL )
    {
        delete lpScaleLevel;
        lpScaleLevel = NULL;
        return NULL;
    }
    lpScaleLevel->m_clrGrid = ExtractColor( pElemChild->FirstChild()->Value() );

    // layer
    pElemChild = pElem->FirstChildElement("layers");
    if ( pElemChild == NULL )
    {
        delete lpScaleLevel;
        lpScaleLevel = NULL;
        return NULL;
    }
    pElemLayer = pElemChild->FirstChildElement( "layer" );
    if ( pElemLayer == NULL )
    {
        delete lpScaleLevel;
        lpScaleLevel = NULL;
        return NULL;
    }
    while ( pElemLayer != NULL )
    {
        LPST_LAYER_CFG lplayer = ImportLayer( pElemLayer );
        if ( lplayer != NULL )
        {
            lpScaleLevel->m_layer_ary.push_back( lplayer );
        }
        pElemLayer = pElemLayer->NextSiblingElement();
    }

    return lpScaleLevel;
}

LPST_LAYER_CFG CMapDisplayCfg::ImportLayer( TiXmlElement* pElem )
{
    if ( pElem == NULL )
    {
        return NULL;
    }
    // <layer id="1" [clone="-1"] geotype="1" featuretype="1" kind="-1" subkind="-1" shpdispmode="3528" namedispmode="3450"/>
    LPST_LAYER_CFG lpLayer = new ST_LAYER_CFG();
    lpLayer->m_nID = atoi( pElem->Attribute("id") );
    if ( pElem->Attribute( "clone" ) != 0 )
    {
        lpLayer->m_nClone = atoi( pElem->Attribute( "clone" ) );
    }
    lpLayer->m_nKind = atoi( pElem->Attribute("kind") );
    lpLayer->m_nSubKind = atoi( pElem->Attribute("subkind") );
    int nGeoType = atoi( pElem->Attribute("geotype") );
    switch ( nGeoType )
    {
    case 1:
        {
            lpLayer->m_geotype = eGeoPoint;
    	    break;
        }
    case 3:
        {
            lpLayer->m_geotype = eGeoLine;
            break;
        }
    case 5:
        {
            lpLayer->m_geotype = eGeoArea;
            break;
        }
    default:
        {
            lpLayer->m_geotype = eGeoInit;
            break;
        }
    }
    int nFeatureType = atoi( pElem->Attribute("featuretype") );
    switch ( nFeatureType )
    {
    case 1:
        {
            lpLayer->m_featuretype = eFeatBk;
            break;
        }
    case 3:
        {
            lpLayer->m_featuretype = eFeatRd;
            break;
        }
    case 5:
        {
            lpLayer->m_featuretype = eFeatTxt;
            break;
        }
    default:
        {
            lpLayer->m_featuretype = eFeatInit;
            break;
        }
    }
    lpLayer->m_nShpDispModeID = atoi(pElem->Attribute( "shpdispmode" ));
    lpLayer->m_nTxtDispModeID = atoi(pElem->Attribute( "namedispmode" ));
    return lpLayer;
}
