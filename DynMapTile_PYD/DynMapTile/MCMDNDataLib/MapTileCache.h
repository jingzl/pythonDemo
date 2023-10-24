/**************************************************************************
FileName:      MapTileCache
Description:   MCMDN ZNMapTile ª∫¥Êπ‹¿Ì¿‡

History:
<author>        <time>          <version >      <desc>
liujingzhou     2013/03/05      1.0             created

*****************************************************************************/
#pragma once
#include "DataType.h"
#include "ZNMapTile.h"
#include "globaldefine.h"



struct st_maptile_cache
{
    ZUINT32 m_nLevel;
    ZUINT32 m_nBlockX;
    ZUINT32 m_nBlockY;
    ZNMapTile* m_pMapTile;
    ZBOOL m_bUse;

    st_maptile_cache()
    {
        m_nLevel = 0;
        m_nBlockX = 0;
        m_nBlockY = 0;
        m_pMapTile = NULL;
        m_bUse = ZFALSE;
    }
    ~st_maptile_cache()
    {
        if ( m_pMapTile != NULL )
        {
            delete m_pMapTile;
            m_pMapTile = NULL;
        }
    }
};


class CMapTileCache
{
public:
    CMapTileCache();
    ~CMapTileCache();

    ZVOID QueryCache( GRIDRECT_LAYER& gridrectlayer, CP_VECTOR<ZNMapTile*>& mapTileAry );

    ZVOID UpdateCache( ZINT32 nLevel, CP_VECTOR<ZNMapTile*>& mapTileAry );


protected:

    ZVOID Release();

private:

    ZINT32 m_nMLevel;

    map<string, st_maptile_cache*> m_MapTileCache_map;

};

