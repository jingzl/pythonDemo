#include "stdafx.h"
#include "AttachDisplayMode.h"



CAttachDisplayMode::CAttachDisplayMode()
{
}

CAttachDisplayMode::~CAttachDisplayMode()
{
    m_PointDisplayMode_map.clear();
    m_LineDisplayMode_map.clear();
    m_AreaDisplayMode_map.clear();
    m_TxtDisplayMode_map.clear();
}

BOOL CAttachDisplayMode::Init( const ST_POINTDISPLAYMODE_ARY* lpPointDisplayModeAry, const ST_LINEDISPLAYMODE_ARY* lpLineDisplayModeAry,
          const ST_AREADISPLAYMODE_ARY* lpAreaDisplayModeAry, const ST_TXTDISPLAYMODE_ARY* lpTxtDisplayModeAry )
{
    if ( lpPointDisplayModeAry == NULL || lpLineDisplayModeAry == NULL || lpAreaDisplayModeAry == NULL || lpTxtDisplayModeAry == NULL )
    {
        return FALSE;
    }

    m_PointDisplayMode_map.clear();
    m_LineDisplayMode_map.clear();
    m_AreaDisplayMode_map.clear();
    m_TxtDisplayMode_map.clear();

    for ( int i=0; i<lpPointDisplayModeAry->size(); i++ )
    {
        LPST_POINTDISPLAYMODE lpPointMode = lpPointDisplayModeAry->at(i);
        if ( lpPointMode != NULL )
        {
            m_PointDisplayMode_map[lpPointMode->m_nID] = lpPointMode;
        }
    }

    for ( int i=0; i<lpLineDisplayModeAry->size(); i++ )
    {
        LPST_LINEDISPLAYMODE lpLineMode = lpLineDisplayModeAry->at(i);
        if ( lpLineMode != NULL )
        {
            m_LineDisplayMode_map[lpLineMode->m_nID] = lpLineMode;
        }
    }

    for ( int i=0; i<lpAreaDisplayModeAry->size(); i++ )
    {
        LPST_AREADISPLAYMODE lpAreaMode = lpAreaDisplayModeAry->at(i);
        if ( lpAreaMode != NULL )
        {
            m_AreaDisplayMode_map[lpAreaMode->m_nID] = lpAreaMode;
        }
    }

    for ( int i=0; i<lpTxtDisplayModeAry->size(); i++ )
    {
        LPST_TXTDISPLAYMODE lpTxtMode = lpTxtDisplayModeAry->at(i);
        if ( lpTxtMode != NULL )
        {
            m_TxtDisplayMode_map[lpTxtMode->m_nID] = lpTxtMode;
        }
    }

    return TRUE;
}

BOOL CAttachDisplayMode::AttachDisplayMode( LPST_LAYER_CFG lpLayerCfg, PBACK_LAYER lplayer )
{
    if ( lpLayerCfg == NULL || lplayer == NULL )
    {
        return FALSE;
    }
    lplayer->type = lpLayerCfg->m_geotype;
    if ( lplayer->type == eGeoLine )
    {
        map<int,LPST_LINEDISPLAYMODE>::iterator it = m_LineDisplayMode_map.find( lpLayerCfg->m_nShpDispModeID );
        if ( it == m_LineDisplayMode_map.end() )
        {
            return FALSE;
        }
        lplayer->lineMode = *(it->second);
    }
    else if ( lplayer->type == eGeoArea )
    {
        map<int,LPST_AREADISPLAYMODE>::iterator it = m_AreaDisplayMode_map.find( lpLayerCfg->m_nShpDispModeID );
        if ( it == m_AreaDisplayMode_map.end() )
        {
            return FALSE;
        }
        lplayer->areaMode = *(it->second);
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

BOOL CAttachDisplayMode::AttachDisplayMode( LPST_LAYER_CFG lpLayerCfg, PROAD_LAYER lplayer )
{
    if ( lpLayerCfg == NULL || lplayer == NULL )
    {
        return FALSE;
    }
    map<int,LPST_LINEDISPLAYMODE>::iterator it1 = m_LineDisplayMode_map.find( lpLayerCfg->m_nShpDispModeID );
    if ( it1 == m_LineDisplayMode_map.end() )
    {
        return FALSE;
    }
    lplayer->lineMode = *(it1->second);
    map<int,LPST_TXTDISPLAYMODE>::iterator it2 = m_TxtDisplayMode_map.find( lpLayerCfg->m_nTxtDispModeID );
    if ( it2 != m_TxtDisplayMode_map.end() )
    {
        // 道路名可能不显示
        lplayer->textMode = *(it2->second);
    }
    return TRUE;
}

BOOL CAttachDisplayMode::AttachDisplayMode( LPST_LAYER_CFG lpLayerCfg, PANNO_LAYER lplayer )
{
    if ( lpLayerCfg == NULL || lplayer == NULL )
    {
        return FALSE;
    }
    map<int,LPST_POINTDISPLAYMODE>::iterator it1 = m_PointDisplayMode_map.find( lpLayerCfg->m_nShpDispModeID );
    if ( it1 == m_PointDisplayMode_map.end() )
    {
        return FALSE;
    }
    lplayer->pointMode = *(it1->second);
    map<int,LPST_TXTDISPLAYMODE>::iterator it2 = m_TxtDisplayMode_map.find( lpLayerCfg->m_nTxtDispModeID );
    if ( it2 != m_TxtDisplayMode_map.end() )
    {
        // 可能名称不显示
        lplayer->textMode = *(it2->second);
    }
    return TRUE;
}

BOOL CAttachDisplayMode::AttachDisplayMode( LPST_LAYER_CFG lpLayerCfg, PPOI_LAYER lplayer )
{
    if ( lpLayerCfg == NULL || lplayer == NULL )
    {
        return FALSE;
    }
    map<int,LPST_POINTDISPLAYMODE>::iterator it1 = m_PointDisplayMode_map.find( lpLayerCfg->m_nShpDispModeID );
    if ( it1 == m_PointDisplayMode_map.end() )
    {
        return FALSE;
    }
    lplayer->pointMode = *(it1->second);

    map<int,LPST_TXTDISPLAYMODE>::iterator it2 = m_TxtDisplayMode_map.find( lpLayerCfg->m_nTxtDispModeID );
    if ( it2 == m_TxtDisplayMode_map.end() )
    {
        return FALSE;
    }
    lplayer->textMode = *(it2->second);
    return TRUE;
}
