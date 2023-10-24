#include "stdafx.h"
#include "MapTileCache.h"



CMapTileCache::CMapTileCache()
{
    m_nMLevel = -1;
}

CMapTileCache::~CMapTileCache()
{
    Release();
}

ZVOID CMapTileCache::QueryCache( GRIDRECT_LAYER& gridrectlayer, CP_VECTOR<ZNMapTile*>& mapTileAry )
{
    if ( gridrectlayer.m_nLevel != m_nMLevel )
    {
        return;
    }

    for ( vector<PGRIDRECT_FEATURE>::iterator it=gridrectlayer.features.begin(); 
        it!=gridrectlayer.features.end(); )
    {
        PGRIDRECT_FEATURE lpGridRect_Feature = *it;
        if ( lpGridRect_Feature == NULL )
        {
            continue;
        }
        char szKey[100] = {0};
        sprintf( szKey, "%d_%d_%d", m_nMLevel, lpGridRect_Feature->m_nTileX, lpGridRect_Feature->m_nTileY );
        string strKey( szKey );
        map<string, st_maptile_cache*>::iterator it2 = m_MapTileCache_map.find( strKey );
        if ( it2 != m_MapTileCache_map.end() )
        {
            st_maptile_cache* lpCache = it2->second;
            if ( lpCache != NULL )
            {
                mapTileAry.push_back( lpCache->m_pMapTile );
                lpCache->m_bUse = ZTRUE;

                delete lpGridRect_Feature;
                lpGridRect_Feature = NULL;
                it = gridrectlayer.features.erase(it);
                continue;
            }
        }
        it++;
    }

    // delete unused element
    for ( map<string, st_maptile_cache*>::iterator it_cache = m_MapTileCache_map.begin(); 
        it_cache != m_MapTileCache_map.end(); )
    {
        st_maptile_cache* lpMapTileCache = it_cache->second;
        if ( lpMapTileCache != NULL && !lpMapTileCache->m_bUse )
        {
            delete lpMapTileCache;
            lpMapTileCache = NULL;
            m_MapTileCache_map.erase(it_cache++);
            continue;
        }
        else
        {
            it_cache++;
        }
        
    }
}

ZVOID CMapTileCache::UpdateCache( ZINT32 nLevel, CP_VECTOR<ZNMapTile*>& mapTileAry )
{
    if ( nLevel != m_nMLevel )
    {
        m_nMLevel = nLevel;
        Release();
    }
    // reset use flag
    for ( map<string, st_maptile_cache*>::iterator it = m_MapTileCache_map.begin(); 
        it != m_MapTileCache_map.end(); it++ )
    {
        st_maptile_cache* lpMapTileCache = it->second;
        if ( lpMapTileCache != NULL )
        {
            lpMapTileCache->m_bUse = ZFALSE;
        }
    }
    for ( CP_VECTOR<ZNMapTile*>::iterator it=mapTileAry.begin(); it!=mapTileAry.end(); it++ )
    {
        ZNMapTile* lpZNMapTile = *it;
        if ( lpZNMapTile == NULL )
        {
            continue;
        }
        st_maptile_cache* lpMapTileCache = new st_maptile_cache();
        lpMapTileCache->m_nLevel = nLevel;
        lpMapTileCache->m_nBlockX = lpZNMapTile->getBlockX();
        lpMapTileCache->m_nBlockY = lpZNMapTile->getBlockY();
        lpMapTileCache->m_pMapTile = lpZNMapTile;

        char szKey[100] = {0};
        sprintf( szKey, "%d_%d_%d", nLevel, lpMapTileCache->m_nBlockX, lpMapTileCache->m_nBlockY );
        string strKey( szKey );
        m_MapTileCache_map[strKey] = lpMapTileCache;
    }
}

//////////////////////////////////////////////////////////////////////////
// 
ZVOID CMapTileCache::Release()
{
    for ( map<string, st_maptile_cache*>::iterator it = m_MapTileCache_map.begin(); 
        it != m_MapTileCache_map.end(); it++ )
    {
        st_maptile_cache* lpMapTileCache = it->second;
        if ( lpMapTileCache != NULL )
        {
            delete lpMapTileCache;
            lpMapTileCache = NULL;
        }
    }
    m_MapTileCache_map.clear();
}

