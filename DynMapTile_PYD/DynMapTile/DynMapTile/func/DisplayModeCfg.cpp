#include "stdafx.h"
#include "DisplayModeCfg.h"



CDisplayModeCfg::CDisplayModeCfg()
{
}

CDisplayModeCfg::~CDisplayModeCfg()
{
    Release();
}

BOOL CDisplayModeCfg::Init( const char* strFilePath )
{
    if ( strlen(strFilePath) <= 0 )
    {
        return FALSE;
    }
    printf( strFilePath );
    printf( "\n" );

    /*
    0x00ff9876;
    RGB(255,1,2);
    byte x=255,y=128,z=128;
    COLORREF clr = RGB(x,y,z);
    */


    Release();
    TiXmlDocument xmlDoc( strFilePath );
    if ( !xmlDoc.LoadFile() )
    {
        return FALSE;
    }
    TiXmlElement* pRootElem = NULL;
    TiXmlElement* pElem = NULL;
    TiXmlElement* pElemChild = NULL;
    pRootElem = xmlDoc.FirstChildElement( "geodisplay_mode" );
    if ( pRootElem == NULL )
    {
        return FALSE;
    }
    int nModeNum = 0;

    // point 
    pElem = pRootElem->FirstChildElement( "point" );
    if ( pElem == NULL )
    {
        return FALSE;
    }
    nModeNum = atoi( pElem->Attribute( "mode_num" ) );
    if ( nModeNum > 0 )
    {
        pElemChild = pElem->FirstChildElement( "dispmode" );
        if ( pElemChild == NULL )
        {
            return FALSE;
        }
        for ( int i=0; i<nModeNum; i++ )
        {
            if ( pElemChild == NULL )
            {
                break;
            }
            LPST_POINTDISPLAYMODE lpmode = ImportPointDisplayMode( pElemChild );
            if ( lpmode != NULL )
            {
                m_PointDisplayMode_ary.push_back( lpmode );
            }
            pElemChild = pElemChild->NextSiblingElement();
        }
    }

    // line 
    pElem = pRootElem->FirstChildElement( "line" );
    if ( pElem == NULL )
    {
        return FALSE;
    }
    nModeNum = atoi( pElem->Attribute( "mode_num" ) );
    if ( nModeNum > 0 )
    {
        pElemChild = pElem->FirstChildElement( "dispmode" );
        if ( pElemChild == NULL )
        {
            return FALSE;
        }
        for ( int i=0; i<nModeNum; i++ )
        {
            if ( pElemChild == NULL )
            {
                break;
            }
            LPST_LINEDISPLAYMODE lpmode = ImportLineDisplayMode( pElemChild );
            if ( lpmode != NULL )
            {
                m_LineDisplayMode_ary.push_back( lpmode );
            }
            pElemChild = pElemChild->NextSiblingElement();
        }
    }

    // area 
    pElem = pRootElem->FirstChildElement( "area" );
    if ( pElem == NULL )
    {
        return FALSE;
    }
    nModeNum = atoi( pElem->Attribute( "mode_num" ) );
    if ( nModeNum > 0 )
    {
        pElemChild = pElem->FirstChildElement( "dispmode" );
        if ( pElemChild == NULL )
        {
            return FALSE;
        }
        for ( int i=0; i<nModeNum; i++ )
        {
            if ( pElemChild == NULL )
            {
                break;
            }
            LPST_AREADISPLAYMODE lpmode = ImportAreaDisplayMode( pElemChild );
            if ( lpmode != NULL )
            {
                m_AreaDisplayMode_ary.push_back( lpmode );
            }
            pElemChild = pElemChild->NextSiblingElement();
        }
    }

    // txt 
    pElem = pRootElem->FirstChildElement( "txt" );
    if ( pElem == NULL )
    {
        return FALSE;
    }
    nModeNum = atoi( pElem->Attribute( "mode_num" ) );
    if ( nModeNum > 0 )
    {
        pElemChild = pElem->FirstChildElement( "dispmode" );
        if ( pElemChild == NULL )
        {
            return FALSE;
        }
        for ( int i=0; i<nModeNum; i++ )
        {
            if ( pElemChild == NULL )
            {
                break;
            }
            LPST_TXTDISPLAYMODE lpmode = ImportTxtDisplayMode( pElemChild );
            if ( lpmode != NULL )
            {
                m_TxtDisplayMode_ary.push_back( lpmode );
            }
            pElemChild = pElemChild->NextSiblingElement();
        }
    }

    return TRUE;
}

void CDisplayModeCfg::Release()
{
    for ( int i=0; i<m_PointDisplayMode_ary.size(); i++ )
    {
        LPST_POINTDISPLAYMODE lpmode = m_PointDisplayMode_ary.at(i);
        if ( lpmode != NULL )
        {
            delete lpmode;
            lpmode = NULL;
        }
    }
    m_PointDisplayMode_ary.clear();

    for ( int i=0; i<m_LineDisplayMode_ary.size(); i++ )
    {
        LPST_LINEDISPLAYMODE lpmode = m_LineDisplayMode_ary.at(i);
        if ( lpmode != NULL )
        {
            delete lpmode;
            lpmode = NULL;
        }
    }
    m_LineDisplayMode_ary.clear();

    for ( int i=0; i<m_AreaDisplayMode_ary.size(); i++ )
    {
        LPST_AREADISPLAYMODE lpmode = m_AreaDisplayMode_ary.at(i);
        if ( lpmode != NULL )
        {
            delete lpmode;
            lpmode = NULL;
        }
    }
    m_AreaDisplayMode_ary.clear();

    for ( int i=0; i<m_TxtDisplayMode_ary.size(); i++ )
    {
        LPST_TXTDISPLAYMODE lpmode = m_TxtDisplayMode_ary.at(i);
        if ( lpmode != NULL )
        {
            delete lpmode;
            lpmode = NULL;
        }
    }
    m_TxtDisplayMode_ary.clear();
}

COLORREF CDisplayModeCfg::ExtractColor( const char* szchar )
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

LPST_POINTDISPLAYMODE CDisplayModeCfg::ImportPointDisplayMode( TiXmlElement* pElem )
{
    if ( pElem == NULL )
    {
        return NULL;
    }
    LPST_POINTDISPLAYMODE lpmode = new ST_POINTDISPLAYMODE();

    int nDrawType = atoi(pElem->Attribute( "drawtype" ));
    switch ( nDrawType )
    {
    case 1:
        {
            lpmode->m_drawtype = ePOINT_DT_TRUETYPE;
            break;
        }
    case 2:
        {
            lpmode->m_drawtype = ePOINT_DT_ICO;
            break;
        }
    case 3:
        {
            lpmode->m_drawtype = ePOINT_DT_BMP;
            break;
        }
    case 4:
        {
            lpmode->m_drawtype = ePOINT_DT_PNG;
            break;
        }
    default:
        {
            lpmode->m_drawtype = ePOINT_DT_INIT;
            break;
        }
    }
    lpmode->m_nSubCode = atoi( pElem->Attribute( "subcode" ) );
    CString strFontName;
    strFontName = pElem->Attribute( "fontname" );
    _tcscpy( lpmode->m_szFontName, strFontName.GetBuffer(strFontName.GetLength()) );
    lpmode->m_clr = ExtractColor( pElem->Attribute( "color" ) );
    lpmode->m_nSize = atoi( pElem->Attribute( "size" ) );
    lpmode->m_nHeight = atoi( pElem->Attribute( "height" ) );
    lpmode->m_nWidth = atoi( pElem->Attribute( "width" ) );
    lpmode->m_nAlfa = atoi( pElem->Attribute("alpha") );
    lpmode->m_nID = atoi( pElem->FirstChild()->Value() );

    return lpmode;
}

LPST_LINEDISPLAYMODE CDisplayModeCfg::ImportLineDisplayMode( TiXmlElement* pElem )
{
    if ( pElem == NULL )
    {
        return NULL;
    }
    LPST_LINEDISPLAYMODE lpmode = new ST_LINEDISPLAYMODE();

    int nDrawType = atoi(pElem->Attribute( "drawtype" ));
    switch ( nDrawType )
    {
    case 1:
        {
            lpmode->m_drawtype = eLINE_DT_RAILWAY;
            break;
        }
    case 2:
        {
            lpmode->m_drawtype = eLINE_DT_SINGLE;
            break;
        }
    case 3:
        {
            lpmode->m_drawtype = eLINE_DT_DOUBLE;
            break;
        }
    default:
        {
            lpmode->m_drawtype = eLINE_DT_INIT;
            break;
        }
    }
    lpmode->m_clrFore = ExtractColor( pElem->Attribute( "forecolor" ) );
    lpmode->m_nWidth = atoi( pElem->Attribute( "width" ) );
    lpmode->m_clrBack = ExtractColor( pElem->Attribute( "backcolor" ) );
    lpmode->m_nEdgeWidth = atoi( pElem->Attribute( "edgewidth" ) );
    lpmode->m_nAlfa = atoi( pElem->Attribute( "alpha" ) );
    lpmode->m_nID = atoi(pElem->FirstChild()->Value());

    return lpmode;
}

LPST_AREADISPLAYMODE CDisplayModeCfg::ImportAreaDisplayMode( TiXmlElement* pElem )
{
    if ( pElem == NULL )
    {
        return NULL;
    }
    LPST_AREADISPLAYMODE lpmode = new ST_AREADISPLAYMODE();

    int nDrawType = atoi(pElem->Attribute( "drawtype" ));
    switch ( nDrawType )
    {
    case 1:
        {
            lpmode->m_drawtype = eAREA_DT_SOLID;
            break;
        }
    case 2:
        {
            lpmode->m_drawtype = eAREA_DT_BMP;
            break;
        }
    default:
        {
            lpmode->m_drawtype = eAREA_DT_INIT;
            break;
        }
    }
    lpmode->m_clrFill = ExtractColor( pElem->Attribute( "fillcolor" ) );
    lpmode->m_nEdgeWidth = atoi( pElem->Attribute( "edgewidth" ) );
    lpmode->m_clrEdge = ExtractColor( pElem->Attribute( "edgecolor" ) );
    lpmode->m_nAlfa = atoi( pElem->Attribute("alpha") );
    lpmode->m_nID = atoi(pElem->FirstChild()->Value());

    return lpmode;
}

LPST_TXTDISPLAYMODE CDisplayModeCfg::ImportTxtDisplayMode( TiXmlElement* pElem )
{
    if ( pElem == NULL )
    {
        return NULL;
    }
    LPST_TXTDISPLAYMODE lpmode = new ST_TXTDISPLAYMODE();

    int nDrawType = atoi(pElem->Attribute( "drawtype" ));
#if 0
    switch ( nDrawType )
    {
    case 1:
        {
            lpmode->m_drawtype = eTXT_DT_SOLID;
            break;
        }
    case 2:
        {
            lpmode->m_drawtype = eTXT_DT_RECT;
            break;
        }
    case 3:
        {
            lpmode->m_drawtype = eTXT_DT_BACK;
            break;
        }
    case 4:
        {
            lpmode->m_drawtype = eTXT_DT_RECTBACK;
            break;
        }
    case 5:
        {
            lpmode->m_drawtype = eTXT_DT_ELLIPSE;
            break;
        }
    default:
        {
            lpmode->m_drawtype = eTXT_DT_INIT;
            break;
        }
    }
#else
	lpmode->m_drawtype = nDrawType;
#endif
    lpmode->m_nSubCode = atoi( pElem->Attribute("subcode") );
    CString strFontName;
    strFontName = pElem->Attribute( "fontname" );
    _tcscpy( lpmode->m_szFontName, strFontName.GetBuffer(strFontName.GetLength()) );
    lpmode->m_nFontSize = atoi( pElem->Attribute( "fontsize" ) );
    lpmode->m_clrFont = ExtractColor( pElem->Attribute( "fontcolor" ) );
    lpmode->m_clrBack = ExtractColor( pElem->Attribute( "backcolor" ) );
    lpmode->m_clrRoundLine = ExtractColor( pElem->Attribute( "roundlinecolor" ) );
    lpmode->m_nEdgeWidth = atoi( pElem->Attribute( "edgewidth" ) );
    int nFlag = 0;
    nFlag = atoi(pElem->Attribute( "bold" ));
    lpmode->m_bBold = (nFlag==1) ? TRUE : FALSE;
    nFlag = atoi(pElem->Attribute( "underline" ));
    lpmode->m_bUnderLine = (nFlag==1) ? TRUE : FALSE;
    nFlag = atoi(pElem->Attribute( "italic" ));
    lpmode->m_bItalic = (nFlag==1) ? TRUE : FALSE;
    nFlag = atoi(pElem->Attribute( "shade" ));
    lpmode->m_bShade = (nFlag==1) ? TRUE : FALSE;
    lpmode->m_nAlfa = atoi( pElem->Attribute( "alpha" ) );
    lpmode->m_nID = atoi(pElem->FirstChild()->Value());

    return lpmode;
}


