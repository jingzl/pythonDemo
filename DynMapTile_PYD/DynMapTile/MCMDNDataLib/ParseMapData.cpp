#include "stdafx.h"
#include <io.h>
#include "ParseMapData.h"
#include "ProjectTrans.h"
#include "MCMDNCoordTrans.h"
#include <WinSock.h>
#pragma comment(lib,"ws2_32")


//extern LogFunc g_lpLogFunc;

CParseMapData* CParseMapData::m_sinstance = NULL;

CParseMapData::CParseMapData():
    m_lpLogFunc(NULL),
    m_strMsg(_T("")),
    m_bInit(FALSE),
    m_lpDisplayModeAry(NULL),
    m_lpPointDispModeAry(NULL),
    m_lpLineDispModeAry(NULL),
    m_lpAreaDispModeAry(NULL),
    m_lpTxtDispModeAry(NULL),
    m_lpScaleCfgAry(NULL),
    m_nMapScaleIndex(0),
    m_nMapScale(0)
{
    memset( m_szDataPath, 0, sizeof(char)*MAX_PATH );
}

CParseMapData* CParseMapData::Instance()
{
    if ( m_sinstance == NULL )
    {
        static CParseMapData parser;
        m_sinstance = &parser;
    }
    return m_sinstance;
}

CParseMapData::~CParseMapData()
{
    Release();
}

void CParseMapData::SetLogFunc( LogFunc lplogfunc )
{
    ASSERT( lplogfunc != NULL );
    m_lpLogFunc = lplogfunc;

    //g_lpLogFunc = lplogfunc;
}

void CParseMapData::SetMapDisplayConfig( const ST_DISPLAYMODE_ARY* lpDisplayModeAry )
{
    ASSERT( lpDisplayModeAry != NULL );
    m_lpDisplayModeAry = lpDisplayModeAry;
}

void CParseMapData::SetDisplayModeConfig( const ST_POINTDISPLAYMODE_ARY* lpPointDisplayModeAry )
{
    ASSERT( lpPointDisplayModeAry != NULL );
    m_lpPointDispModeAry = lpPointDisplayModeAry;
}

void CParseMapData::SetDisplayModeConfig( const ST_LINEDISPLAYMODE_ARY* lpLineDisplayModeAry )
{
    ASSERT( lpLineDisplayModeAry != NULL );
    m_lpLineDispModeAry = lpLineDisplayModeAry;
}

void CParseMapData::SetDisplayModeConfig( const ST_AREADISPLAYMODE_ARY* lpAreaDisplayModeAry )
{
    ASSERT( lpAreaDisplayModeAry != NULL );
    m_lpAreaDispModeAry = lpAreaDisplayModeAry;
}

void CParseMapData::SetDisplayModeConfig( const ST_TXTDISPLAYMODE_ARY* lpTxtDisplayModeAry )
{
    ASSERT( lpTxtDisplayModeAry != NULL );
    m_lpTxtDispModeAry = lpTxtDisplayModeAry;
}

void CParseMapData::SetMapScaleConfig( ST_SCALECFG_ARY* scalecfgary )
{
    ASSERT( scalecfgary != NULL );
    m_lpScaleCfgAry = scalecfgary;
}

BOOL CParseMapData::Init( const char* lpszDataPath )
{
    if ( lpszDataPath == NULL || strlen(lpszDataPath) <= 0 )
    {
        return FALSE;
    }
    strcpy( m_szDataPath, lpszDataPath );
    if ( !m_AttachDisplayMode.Init( m_lpPointDispModeAry, m_lpLineDispModeAry, m_lpAreaDispModeAry, m_lpTxtDispModeAry ) )
    {
        m_strMsg.Format( _T("AttachDisplayMode - init fail !") );
        OutputMessage( m_strMsg );
        return FALSE;
    }
    // road name id : 72 ~ 81
    for ( int nLayerID=72; nLayerID<=81; nLayerID++ )
    {
        m_RoadNameLayerID_ary.push_back( nLayerID );
    }
    m_bInit = TRUE;
    return TRUE;
}

BOOL CParseMapData::GetMapData( int nDisplayMode, BYTE nMapScale, GEORECT rect, PMAP_DISP_DATA& lpMapDispData )
{
    if ( !m_bInit )
    {
        m_strMsg.Format( _T("Parse not Init !") );
        OutputMessage( m_strMsg );
        return FALSE;
    }
    m_nMapScale = nMapScale;
    if ( nDisplayMode < 0 || nDisplayMode >= m_lpDisplayModeAry->size() )
    {
        m_strMsg.Format( _T("DisplayMode - %d error !"), nDisplayMode );
        OutputMessage( m_strMsg );
        return FALSE;
    }
    LPST_DISPLAYMODE lpDisplayMode = (*m_lpDisplayModeAry)[nDisplayMode];
    if ( lpDisplayMode == NULL )
    {
        return FALSE;
    }
    m_nMapScaleIndex = GetMapScaleIndex( m_nMapScale );
    if ( m_nMapScaleIndex < 0 || m_nMapScaleIndex >= lpDisplayMode->m_scalelevel_ary.size() )
    {
        m_strMsg.Format( _T("MapScaleIndex - %d error !"), m_nMapScaleIndex );
        OutputMessage( m_strMsg );
        return FALSE;
    }
    LPST_SCALELEVEL lpLevel = lpDisplayMode->m_scalelevel_ary[m_nMapScaleIndex];
    if ( lpLevel == NULL )
    {
        return FALSE;
    }
    if ( lpMapDispData != NULL )
    {
        return FALSE;
    }
    lpMapDispData = new MAP_DISP_DATA();
    lpMapDispData->clrback = lpLevel->m_clrGround;
    m_GeoRect = rect;

    // 计算获得格网信息
    if ( !GetGridRectData( lpMapDispData->gridrectLayer ) )
    {
        return FALSE;
    }

    CP_VECTOR<ZNMapTile*> mapTileAry;
    // 查询缓存
    m_MapTileCache.QueryCache( lpMapDispData->gridrectLayer, mapTileAry );

    CP_VECTOR<ZNMapTile*> mapTileAry2;
    if ( !lpMapDispData->gridrectLayer.features.empty() )
    {
        if ( !GetMCMDNData( lpMapDispData->gridrectLayer, mapTileAry2 ) )
        {
            return FALSE;
        }
    }

    mapTileAry.insert( mapTileAry.end(), mapTileAry2.begin(), mapTileAry2.end() );
    if ( !Trans2DisplayData( lpLevel, mapTileAry, lpMapDispData ) )
    {
        return FALSE;
    }

    // 更新缓存
    m_MapTileCache.UpdateCache( lpMapDispData->gridrectLayer.m_nLevel, mapTileAry2 );
    mapTileAry2.clear();
    mapTileAry.clear();

    /*
    // release MapTileAry
    for ( CP_VECTOR<ZNMapTile*>::iterator it=mapTileAry.begin(); it!=mapTileAry.end(); it++ )
    {
        ZNMapTile* lpZNMapTile = *it;
        if ( lpZNMapTile != NULL )
        {
            delete lpZNMapTile;
            lpZNMapTile = NULL;
        }
    }
    mapTileAry.clear();
    */

    return TRUE;
}

void CParseMapData::ReleaseMapData( PMAP_DISP_DATA lpMapDispData )
{
    if ( lpMapDispData != NULL )
    {
        delete lpMapDispData;
        lpMapDispData = NULL;
    }
}

void CParseMapData::Release()
{
    m_lpLogFunc = NULL;
    m_bInit = FALSE;
    m_lpDisplayModeAry = NULL;
    m_lpPointDispModeAry = NULL;
    m_lpLineDispModeAry = NULL;
    m_lpAreaDispModeAry = NULL;
    m_lpTxtDispModeAry = NULL;
    m_lpScaleCfgAry = NULL;
    m_nMapScaleIndex = 0;
    m_RoadNameLayerID_ary.clear();
}

//////////////////////////////////////////////////////////////////////////
void CParseMapData::OutputMessage( CString str )
{
    if ( m_lpLogFunc != NULL )
    {
        m_lpLogFunc( str );
    }
}

int CParseMapData::GetMapScaleIndex( int nScale )
{
    if ( m_lpScaleCfgAry == NULL )
    {
        return -1;
    }
    for ( ZINT32 i=0; i<m_lpScaleCfgAry->size(); i++ )
    {
        LPST_SCALECFG lpScaleCfg = m_lpScaleCfgAry->at(i);
        if ( lpScaleCfg == NULL )
        {
            continue;
        }
        if ( lpScaleCfg->m_nScale == nScale )
        {
            return i;
        }
    }
    return -1;
}

BOOL CParseMapData::GetGridRectData( GRIDRECT_LAYER& gridrectlayer )
{
    gridrectlayer.m_nZoom = m_nMapScale;

    // lineMode
    gridrectlayer.lineMode.m_nID = 10;
    gridrectlayer.lineMode.m_drawtype = eLINE_DT_SINGLE;
    gridrectlayer.lineMode.m_clrFore = RGB(255,0,0);
    gridrectlayer.lineMode.m_nWidth = 1;
    gridrectlayer.lineMode.m_clrBack = RGB(255,0,0);
    gridrectlayer.lineMode.m_nEdgeWidth = 0;
    gridrectlayer.lineMode.m_nAlfa = 100;

    /*
    if ( !GetGoogleTileRect( gridrectlayer ) )
    {
        return FALSE;
    }*/
    
    if ( !GetMCMDNRect( gridrectlayer ) )
    {
        return FALSE;
    }


    return TRUE;
}

BOOL CParseMapData::GetMCMDNRect( GRIDRECT_LAYER& gridrectlayer )
{
    if ( m_lpScaleCfgAry->size() <= m_nMapScaleIndex )
    {
        return FALSE;
    }
    gridrectlayer.m_nLevel = (*m_lpScaleCfgAry)[m_nMapScaleIndex]->m_nMLevel;

    // 左下 --> 右上
    int nMinBlockX = 0;
    int nMinBlockY = 0;
    int nMaxBlockX = 0;
    int nMaxBlockY = 0;
    CMCMDNCoordTrans::ZNMapGetXYOfLL( gridrectlayer.m_nLevel, TRANS_TO_SECOND1000(m_GeoRect.dMinX), TRANS_TO_SECOND1000(m_GeoRect.dMinY), 
        nMinBlockX, nMinBlockY );
    CMCMDNCoordTrans::ZNMapGetXYOfLL( gridrectlayer.m_nLevel, TRANS_TO_SECOND1000(m_GeoRect.dMaxX), TRANS_TO_SECOND1000(m_GeoRect.dMaxY), 
        nMaxBlockX, nMaxBlockY );

    //nMinBlockX	11297	int
    //nMinBlockY	7399	int
    //nMaxBlockX	11320	int
    //nMaxBlockY	7425	int

    for ( int ix=nMinBlockX; ix<=nMaxBlockX; ix++ )
    {
        for ( int iy=nMinBlockY; iy<=nMaxBlockY; iy++ )
        {
            /*if ( ix != 9487 || iy != 6241 )
            {
                continue;
            }*/
            PGRIDRECT_FEATURE lpGridRectFeature = new GRIDRECT_FEATURE();
            if ( lpGridRectFeature == NULL )
            {
                return FALSE;
            }
            int nMinX = 0;
            int nMinY = 0;
            int nMaxX = 0;
            int nMaxY = 0;
            CMCMDNCoordTrans::ZNMapGetLLOfBlockRect( gridrectlayer.m_nLevel, ix, iy, nMinX, nMinY, nMaxX, nMaxY );
            lpGridRectFeature->m_rcFeatureRect.dMinX = REVTRANS_TO_SECOND1000(nMinX);
            lpGridRectFeature->m_rcFeatureRect.dMinY = REVTRANS_TO_SECOND1000(nMinY);
            lpGridRectFeature->m_rcFeatureRect.dMaxX = REVTRANS_TO_SECOND1000(nMaxX);
            lpGridRectFeature->m_rcFeatureRect.dMaxY = REVTRANS_TO_SECOND1000(nMaxY);

            lpGridRectFeature->m_wptCount = (WORD)5;
            lpGridRectFeature->m_pShape = new GEOPOINT[lpGridRectFeature->m_wptCount];

            lpGridRectFeature->m_pShape[0].x = lpGridRectFeature->m_rcFeatureRect.dMinX;
            lpGridRectFeature->m_pShape[0].y = lpGridRectFeature->m_rcFeatureRect.dMinY;

            lpGridRectFeature->m_pShape[1].x = lpGridRectFeature->m_rcFeatureRect.dMaxX;
            lpGridRectFeature->m_pShape[1].y = lpGridRectFeature->m_rcFeatureRect.dMinY;

            lpGridRectFeature->m_pShape[2].x = lpGridRectFeature->m_rcFeatureRect.dMaxX;
            lpGridRectFeature->m_pShape[2].y = lpGridRectFeature->m_rcFeatureRect.dMaxY;

            lpGridRectFeature->m_pShape[3].x = lpGridRectFeature->m_rcFeatureRect.dMinX;
            lpGridRectFeature->m_pShape[3].y = lpGridRectFeature->m_rcFeatureRect.dMaxY;

            lpGridRectFeature->m_pShape[4] = lpGridRectFeature->m_pShape[0];

            lpGridRectFeature->m_nTileX = ix;
            lpGridRectFeature->m_nTileY = iy;
            gridrectlayer.features.push_back( lpGridRectFeature );
        }
    }

    return TRUE;
}

BOOL CParseMapData::GetGoogleTileRect( GRIDRECT_LAYER& gridrectlayer )
{
    CProjectTrans projtrans;

    // LL --> PixelXY --> TileXY
    int px1, px2, py1, py2;
    // 左上角
    projtrans.LongLatToPixelXY( m_GeoRect.dMinX, m_GeoRect.dMaxY, m_nMapScaleIndex, px1, py1 );
    // 右下角
    projtrans.LongLatToPixelXY( m_GeoRect.dMaxX, m_GeoRect.dMinY, m_nMapScaleIndex, px2, py2 );

    int nMinCol = 0;
    int nMaxCol = 0;
    int nMinRow = 0;
    int nMaxRow = 0;
    bool bx1 = false;
    bool by1 = false;
    bool bx2 = false;
    bool by2 = false;
    projtrans.pixelXYToTileXY( px1, py1, nMinCol, nMinRow, bx1, by1 );    
    projtrans.pixelXYToTileXY( px2, py2, nMaxCol, nMaxRow, bx2, by2 );
    // 只针对右下角进行处理
    if ( bx2 )
    {
        nMaxCol -= 1;
    }
    if ( by2 )
    {
        nMaxRow -= 1;
    }

    // 左上 --> 右下
    // 列
    for ( int ix=nMinCol; ix<=nMaxCol; ix++ )
    {
        // 行
        for ( int iy=nMinRow; iy<=nMaxRow; iy++ )
        {
            // 
            PGRIDRECT_FEATURE lpGridRectFeature = new GRIDRECT_FEATURE();
            if ( lpGridRectFeature == NULL )
            {
                return FALSE;
            }
            projtrans.tileXYToLLBound( ix, iy, gridrectlayer.m_nZoom, 
                lpGridRectFeature->m_rcFeatureRect.dMinX, lpGridRectFeature->m_rcFeatureRect.dMaxY, 
                lpGridRectFeature->m_rcFeatureRect.dMaxX, lpGridRectFeature->m_rcFeatureRect.dMinY );

            lpGridRectFeature->m_wptCount = (WORD)5;
            lpGridRectFeature->m_pShape = new GEOPOINT[lpGridRectFeature->m_wptCount];

            lpGridRectFeature->m_pShape[0].x = lpGridRectFeature->m_rcFeatureRect.dMinX;
            lpGridRectFeature->m_pShape[0].y = lpGridRectFeature->m_rcFeatureRect.dMinY;

            lpGridRectFeature->m_pShape[1].x = lpGridRectFeature->m_rcFeatureRect.dMaxX;
            lpGridRectFeature->m_pShape[1].y = lpGridRectFeature->m_rcFeatureRect.dMinY;

            lpGridRectFeature->m_pShape[2].x = lpGridRectFeature->m_rcFeatureRect.dMaxX;
            lpGridRectFeature->m_pShape[2].y = lpGridRectFeature->m_rcFeatureRect.dMaxY;

            lpGridRectFeature->m_pShape[3].x = lpGridRectFeature->m_rcFeatureRect.dMinX;
            lpGridRectFeature->m_pShape[3].y = lpGridRectFeature->m_rcFeatureRect.dMaxY;

            lpGridRectFeature->m_pShape[4] = lpGridRectFeature->m_pShape[0];

            lpGridRectFeature->m_nTileX = ix;
            lpGridRectFeature->m_nTileY = iy;
            gridrectlayer.features.push_back( lpGridRectFeature );
        }
    }

    return TRUE;
}



struct st_MeshInfo
{
    unsigned short mx;
    unsigned short my;
    unsigned int offset;

    st_MeshInfo()
    {
        mx = 0;
        my = 0;
        offset = -1;
    }
    ~st_MeshInfo()
    {
        // 
    }
};




BOOL CParseMapData::GetMCMDNData( GRIDRECT_LAYER& gridrectlayer, CP_VECTOR<ZNMapTile*>& mapTileAry )
{
    CP_VECTOR<STSingleBlockDataInfo*> blockDataInfoAry;
    int nLevel = gridrectlayer.m_nLevel;
    char cLevel = CMCMDNCoordTrans::GetMCMDNLevel( nLevel );
    for ( vector<PGRIDRECT_FEATURE>::iterator it=gridrectlayer.features.begin(); it!=gridrectlayer.features.end(); it++ )
    {
        PGRIDRECT_FEATURE lpGridRectFeature = *it;
        int nBlockX = lpGridRectFeature->m_nTileX;
        int nBlockY = lpGridRectFeature->m_nTileY;
        int nMeshX = 0;
        int nMeshY = 0;
        //if ( (nBlockX != 2374 || nBlockY != 1562) && (nBlockX != 2374 || nBlockY != 1563) )
        //{
        //    continue;
        //}

        int nCenterX = TRANS_TO_SECOND1000((lpGridRectFeature->m_rcFeatureRect.dMinX+lpGridRectFeature->m_rcFeatureRect.dMaxX)/2);
        int nCenterY = TRANS_TO_SECOND1000((lpGridRectFeature->m_rcFeatureRect.dMinY+lpGridRectFeature->m_rcFeatureRect.dMaxY)/2);
        CMCMDNCoordTrans::ZNMapGetMeshXYOfLL( nLevel, nCenterX, nCenterY, nMeshX, nMeshY );

        // 读取文件
        char szFileName[128] = {0};
        char szFilePath[255] = {0};
#if 0
        CMCMDNCoordTrans::GetMCMDNBlockFilePath( nLevel, nMeshX, nMeshY, nBlockX, nBlockY, szFileName );
        sprintf( szFilePath, ("%s\\%s"), "d:", szFileName );
        FILE* fp = NULL;
        if ( (fp = fopen( szFilePath, "rb" )) == NULL )
        {
            continue;
        }
        unsigned int blockOffset = 0;
        size_t size = 0;
#else
        CMCMDNCoordTrans::GetMCMDNFilePath( nLevel, nMeshX, nMeshY, szFileName );
        sprintf( szFilePath, ("%s\\%s"), m_szDataPath, szFileName );
        
        FILE* fp = NULL;
        fp = fopen( szFilePath, "rb" );
        if ( fp == NULL )
        {
            CString strMsg;
            strMsg.Format( _T("%c00\\%04x\\%04x\\%04x%04x.mm"), cLevel, nMeshX/16, nMeshY/16, nMeshX/4, nMeshY/4 );
            OutputMessage( strMsg );
            continue;
        }

        // 每个文件中包含16个mesh，每个mesh中包含16个block数据

        // File Header
        // 跳过文件头
        char szFileHeader[16] = {0};
        fread( szFileHeader, 16, 1, fp );

        // 管理部
        //vector<st_MeshInfo> meshInfo_ary;
        st_MeshInfo meshinfo;
        BOOL bFind = FALSE;
        int nMapDataOffset = ftell( fp );
        nMapDataOffset += 16 * 8;
        for ( int i=0; i<16; i++ )
        {
            fread( &meshinfo.mx, 2, 1, fp );
            meshinfo.mx = ntohs( meshinfo.mx );
            fread( &meshinfo.my, 2, 1, fp );
            meshinfo.my = ntohs( meshinfo.my );
            fread( &meshinfo.offset, 4, 1, fp );
            meshinfo.offset = ntohl( meshinfo.offset );
            //meshInfo_ary.push_back( meshinfo );
            if ( meshinfo.mx == nMeshX && meshinfo.my == nMeshY )
            {
                bFind = TRUE;
                break;
            }
        }
        if ( !bFind )
        {
            fclose( fp );
            fp = NULL;
            continue;
        }

        meshinfo.offset += nMapDataOffset;
        fseek( fp, meshinfo.offset, SEEK_SET );

        int nBlockOffset = meshinfo.offset;
        nBlockOffset += 4 * 16 + 4;
        // 解析16个block的offset信息
        vector<unsigned int> block_offset_ary;
        for ( int i=0; i<16; i++ )
        {
            unsigned int offset = -1;
            fread( &offset, 4, 1, fp );
            offset = ntohl( offset );
            offset += nBlockOffset;
            block_offset_ary.push_back( offset );
        }
        unsigned int size = 0;
        fread( &size, 4, 1, fp );
        size = ntohl( size );

        CString str;
        int nIdx = CMCMDNCoordTrans::ZNMapGetBlockIdxOfMesh( nMeshX, nMeshY, nBlockX, nBlockY );
        if ( nIdx == -1 )
        {
            //str.Format( _T("%d-%d-%d-%d"), nMeshX, nMeshY, nBlockX, nBlockY );
            //OutputMessage( str );
            fclose( fp );
            fp = NULL;
            continue;
        }
        unsigned int blockOffset = block_offset_ary[nIdx];
        if ( blockOffset == 0xFFFFFFFF )
        {
            //str.Format( _T("%d-%d-%d-%d"), nMeshX, nMeshY, nBlockX, nBlockY );
            //OutputMessage( str );
            fclose( fp );
            fp = NULL;
            continue;
        }
#endif
        fseek( fp, blockOffset, SEEK_SET );
        fseek( fp, 10, SEEK_CUR );
        unsigned short binfosize = 0;
        fread( &binfosize, 2, 1, fp );
        binfosize = ntohs( binfosize );
        binfosize = 2 * binfosize;
        if ( binfosize <= 4 )
        {
            //str.Format( _T("%d-%d-%d-%d"), nMeshX, nMeshY, nBlockX, nBlockY );
            //OutputMessage( str );
            fclose( fp );
            fp = NULL;
            continue;
        }

        STSingleBlockDataInfo* pBlockDataInfo = new STSingleBlockDataInfo();
        pBlockDataInfo->pBlcokDataInfo = new ZBYTE[binfosize+12];
        fseek( fp, blockOffset, SEEK_SET );
        size = fread( pBlockDataInfo->pBlcokDataInfo, 1, binfosize+12, fp );
        char szID[255] = {0};
        sprintf( szID, ("%c%d.%d"), cLevel, nBlockX, nBlockY );
        char* pID = new char[strlen(szID)+1];
        memset( pID, 0, strlen(szID)+1 );
        strcpy( pID, szID );
        pBlockDataInfo->pTileID = (ZBYTE*)pID;
        pBlockDataInfo->ulBlcokDataLen = size;
        blockDataInfoAry.push_back( pBlockDataInfo );

        /*
        for ( int i=0; i<16; i++ )
        {
            unsigned int offset = block_offset_ary[i];
            if ( offset == 0xFFFFFFFF )
            {
                continue;
            }
            fseek( fp, offset, SEEK_SET );

            unsigned char sig = -1;
            fread( &sig, 1, 1, fp );
            unsigned char kind = -1;
            fread( &kind, 1, 1, fp );
            unsigned int blockX = 0;
            unsigned int blockY = 0;
            fread( &blockX, 4, 1, fp );
            fread( &blockY, 4, 1, fp );
            blockX = ntohl( blockX );
            blockY = ntohl( blockY );

            unsigned short bsize = 0;
            fread( &bsize, 2, 1, fp );
            bsize = ntohs( bsize );

            int mm = 0;
        }
        */
        

        fclose( fp );
        fp = NULL;
    }
    ZNMapDecoder mapDecoder = ZNMapDecoder::getInstance();
    mapDecoder.decodeBlockData( &blockDataInfoAry, &mapTileAry );

    // delete blockdatainfo
    for ( CP_VECTOR<STSingleBlockDataInfo*>::iterator it=blockDataInfoAry.begin(); it!=blockDataInfoAry.end(); it++ )
    {
        STSingleBlockDataInfo* lpBlockDataInfo = *it;
        if ( lpBlockDataInfo != NULL )
        {
            delete [] lpBlockDataInfo->pBlcokDataInfo;
            lpBlockDataInfo->pBlcokDataInfo = NULL;
            delete [] lpBlockDataInfo->pTileID;
            lpBlockDataInfo->pTileID = NULL;
            delete lpBlockDataInfo;
            lpBlockDataInfo = NULL;
        }
    }
    blockDataInfoAry.clear();

    return TRUE;
}

BOOL CParseMapData::Trans2DisplayData( LPST_SCALELEVEL lpLevel, CP_VECTOR<ZNMapTile*>& mapTileAry, PMAP_DISP_DATA& lpMapDispData )
{
    if ( lpLevel == NULL )
    {
        return FALSE;
    }

    // 提取道路的所有名称信息
    if ( !ExtractRoadName( mapTileAry ) )
    {
        m_strMsg.Format( _T("extract road name fail !") );
        OutputMessage( m_strMsg );
        return FALSE;
    }

    // 根据配置图层转换
    for ( ST_LAYER_CFG_ARY::iterator it=lpLevel->m_layer_ary.begin(); it!=lpLevel->m_layer_ary.end(); it++ )
    {
        LPST_LAYER_CFG lpLayerCfg = *it;
        if ( lpLayerCfg == NULL )
        {
            continue;
        }

        if ( lpLayerCfg->m_nClone != -1 && lpLayerCfg->m_featuretype == eFeatRd )
        {
            // 处理道路要素的clone属性
            PROAD_LAYER lpRoadLayer = NULL;
            lpRoadLayer = CloneRoadLayer( lpLayerCfg->m_nClone );
            if ( lpRoadLayer == NULL )
            {
                continue;
            }
            lpRoadLayer->layerID = lpLayerCfg->m_nID;
            m_RoadLayer_map[lpLayerCfg->m_nID] = lpRoadLayer;
        }
        else if ( lpLayerCfg->m_nClone != -1 && lpLayerCfg->m_featuretype == eFeatTxt )
        {
            // 处理注记要素的clone属性
            PANNO_LAYER lpAnnoLayer = NULL;
            lpAnnoLayer = CloneAnnoLayer( lpLayerCfg->m_nClone, lpLayerCfg->m_nKind, lpLayerCfg->m_nSubKind );
            if ( lpAnnoLayer == NULL )
            {
                continue;
            }
            lpAnnoLayer->layerID = lpLayerCfg->m_nID;
            m_AnnoLayer_map[lpLayerCfg->m_nID] = lpAnnoLayer;
        }
        else
        {
            for ( CP_VECTOR<ZNMapTile*>::iterator it=mapTileAry.begin(); it!=mapTileAry.end(); it++ )
            {
                ZNMapTile* pZNMapTile = *it;
                if ( pZNMapTile == NULL )
                {
                    continue;
                }
                CP_VECTOR<ZNMapLayer*> znMapLayers = pZNMapTile->getLayers();
                for ( CP_VECTOR<ZNMapLayer*>::iterator it_layer=znMapLayers.begin(); it_layer!=znMapLayers.end(); it_layer++ )
                {
                    ZNMapLayer* pZNMapLayer = *it_layer;
                    if ( pZNMapLayer == NULL || pZNMapLayer->mMapDataRep == NULL )
                    {
                        continue;
                    }
                    if ( pZNMapLayer->mNumber != lpLayerCfg->m_nID )
                    {
                        continue;
                    }
                    if ( lpLayerCfg->m_featuretype == eFeatBk )
                    {
                        if ( lpLayerCfg->m_geotype == eGeoLine )
                        {
                            if ( pZNMapLayer->mMapDataRep->m_lineShapes.size() <= 0 )
                            {
                                continue;
                            } 
                        }
                        else if ( lpLayerCfg->m_geotype == eGeoArea )
                        {
                            if ( pZNMapLayer->mMapDataRep->m_polygonShapes.size() <= 0 && 
                                pZNMapLayer->mMapDataRep->m_polygonShape2s.size() <= 0 && 
                                pZNMapLayer->mMapDataRep->m_polygonShape3s.size() <= 0 && 
                                pZNMapLayer->mMapDataRep->m_polygonShape4s.size() <= 0 && 
                                pZNMapLayer->mMapDataRep->m_polygonShape5s.size() <= 0 && 
                                pZNMapLayer->mMapDataRep->m_polygonShape6s.size() <= 0 )
                            {
                                continue;
                            }
                        }
                        if ( !TransBK( lpLayerCfg, pZNMapLayer ) )
                        {
                            return FALSE;
                        }
                    }
                    else if ( lpLayerCfg->m_featuretype == eFeatRd )
                    {
                        if ( pZNMapLayer->mMapDataRep->m_lineShapes.size() <= 0 )
                        {
                            continue;
                        }
                        if ( !TransRoad( lpLayerCfg, pZNMapLayer ) )
                        {
                            return FALSE;
                        }
                    }
                    else if ( lpLayerCfg->m_featuretype == eFeatTxt )
                    {
                        if ( pZNMapLayer->mMapDataRep->m_textShapes.size() <= 0 && 
                            pZNMapLayer->mMapDataRep->m_signImageShapes.size() <= 0 && 
                            pZNMapLayer->mMapDataRep->m_signTextShapes.size() <= 0 )
                        {
                            continue;
                        }
                        if ( !TransAnno( lpLayerCfg, pZNMapLayer ) )
                        {
                            return FALSE;
                        }
                    }
                    // 
                }
                // 
            }
        }

        // 附加显示属性
        if ( lpLayerCfg->m_featuretype == eFeatBk && m_BackLayer_map.size() > 0 )
        {
            map<int,PBACK_LAYER>::iterator it_layer = m_BackLayer_map.find( lpLayerCfg->m_nID );
            if ( it_layer != m_BackLayer_map.end() )
            {
                PBACK_LAYER lpBackLayer = it_layer->second;
                if ( !m_AttachDisplayMode.AttachDisplayMode( lpLayerCfg, lpBackLayer ) )
                {
                    m_strMsg.Format( _T("AttachDisplayMode - backlayer fail !") );
                    OutputMessage( m_strMsg );
                    return FALSE;
                }
                lpMapDispData->backLayers.push_back( lpBackLayer );
            }
        }
        else if ( lpLayerCfg->m_featuretype == eFeatRd && m_RoadLayer_map.size() > 0 )
        {
            map<int,PROAD_LAYER>::iterator it_layer = m_RoadLayer_map.find( lpLayerCfg->m_nID );
            if ( it_layer != m_RoadLayer_map.end() )
            {
                PROAD_LAYER lpRoadLayer = it_layer->second;
                if ( !m_AttachDisplayMode.AttachDisplayMode( lpLayerCfg, lpRoadLayer ) )
                {
                    m_strMsg.Format( _T("AttachDisplayMode - roadlayer fail !") );
                    OutputMessage( m_strMsg );
                    return FALSE;
                }
                lpMapDispData->roadLayers.push_back( lpRoadLayer ); 
            }
        }
        else if ( lpLayerCfg->m_featuretype == eFeatTxt && m_AnnoLayer_map.size() > 0 )
        {
            map<int,PANNO_LAYER>::iterator it_layer = m_AnnoLayer_map.find( lpLayerCfg->m_nID );
            if ( it_layer != m_AnnoLayer_map.end() )
            {
                PANNO_LAYER lpAnnoLayer = it_layer->second;
                if ( !m_AttachDisplayMode.AttachDisplayMode( lpLayerCfg, lpAnnoLayer ) )
                {
                    m_strMsg.Format( _T("AttachDisplayMode - namelayer fail !") );
                    OutputMessage( m_strMsg );
                    return FALSE;
                }
                lpMapDispData->annoLayers.push_back( lpAnnoLayer ); 
            }
        }
    }

    m_BackLayer_map.clear();
    m_RoadLayer_map.clear();
    m_AnnoLayer_map.clear();
    m_RoadName_map.clear();

    return TRUE;
}

BOOL CParseMapData::ExtractRoadName( CP_VECTOR<ZNMapTile*>& mapTileAry )
{
    // 道路名称的Layer ID 范围：72 ~ 81 
    for ( CP_VECTOR<ZNMapTile*>::iterator it=mapTileAry.begin(); it!=mapTileAry.end(); it++ )
    {
        ZNMapTile* pZNMapTile = *it;
        if ( pZNMapTile == NULL )
        {
            continue;
        }
        CP_VECTOR<ZNMapLayer*> znMapLayers = pZNMapTile->getLayers();
        for ( CP_VECTOR<ZNMapLayer*>::iterator it_layer=znMapLayers.begin(); it_layer!=znMapLayers.end(); it_layer++ )
        {
            ZNMapLayer* pZNMapLayer = *it_layer;
            if ( pZNMapLayer == NULL || pZNMapLayer->mMapDataRep == NULL )
            {
                continue;
            }
            vector<int>::iterator it_ID = find( m_RoadNameLayerID_ary.begin(), m_RoadNameLayerID_ary.end(), pZNMapLayer->mNumber );
            if ( it_ID == m_RoadNameLayerID_ary.end() )
            {
                continue;
            }
            if ( pZNMapLayer->mMapDataRep->m_textShapes.size() <= 0 )
            {
                continue;
            }
            for ( CP_VECTOR<ZNMapTextShape*>::iterator it_text=pZNMapLayer->mMapDataRep->m_textShapes.begin(); 
                it_text!=pZNMapLayer->mMapDataRep->m_textShapes.end(); it_text++ )
            {
                ZNMapTextShape* pZNMapTextShape = *it_text;
                if ( pZNMapTextShape == NULL )
                {
                    continue;
                }
                if ( pZNMapTextShape->m_textArray.size() <= 0 )
                {
                    continue;
                }
                wstring wstr = pZNMapTextShape->m_textArray[0];
                if ( wstr.size() < 2 )
                {
                    continue;
                }
                m_RoadName_map[pZNMapTextShape->m_textData.mID] = wstr;
            }
        }
    }

    return TRUE;
}

BOOL CParseMapData::TransBK( LPST_LAYER_CFG lpLayerCfg, ZNMapLayer* pZNMapLayer )
{
    PBACK_LAYER lpBackLayer = NULL;
    map<int,PBACK_LAYER>::iterator it_layer = m_BackLayer_map.find( pZNMapLayer->mNumber );
    if ( it_layer != m_BackLayer_map.end() )
    {
        lpBackLayer = it_layer->second;
    }
    else
    {
        lpBackLayer = new BACK_LAYER();
        m_BackLayer_map[pZNMapLayer->mNumber] = lpBackLayer;
    }

    if ( lpLayerCfg->m_geotype == eGeoLine )
    {
        // 背景线：行政区划边界线、建筑物边界线等
        if ( !TransLineShape( lpBackLayer, pZNMapLayer ) )
        {
            return FALSE;
        }
    }
    else if ( lpLayerCfg->m_geotype == eGeoArea )
    {
        // 背景面：水系、绿地、建筑区域等
        if ( !TransPolygonShape( lpBackLayer, pZNMapLayer ) )
        {
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CParseMapData::TransRoad( LPST_LAYER_CFG lpLayerCfg, ZNMapLayer* pZNMapLayer )
{
    PROAD_LAYER lpRoadLayer = NULL;
    map<int,PROAD_LAYER>::iterator it_layer = m_RoadLayer_map.find( pZNMapLayer->mNumber );
    if ( it_layer != m_RoadLayer_map.end() )
    {
        lpRoadLayer = it_layer->second;
    }
    else
    {
        lpRoadLayer = new ROAD_LAYER();
        m_RoadLayer_map[pZNMapLayer->mNumber] = lpRoadLayer;
    }
    if ( !TransLineShape( lpRoadLayer, pZNMapLayer ) )
    {
        return FALSE;
    }

    return TRUE;
}

BOOL CParseMapData::TransAnno( LPST_LAYER_CFG lpLayerCfg, ZNMapLayer* pZNMapLayer )
{
    PANNO_LAYER lpAnnoLayer = NULL;
    map<int,PANNO_LAYER>::iterator it_layer = m_AnnoLayer_map.find( pZNMapLayer->mNumber );
    if ( it_layer != m_AnnoLayer_map.end() )
    {
        lpAnnoLayer = it_layer->second;
    }
    else
    {
        lpAnnoLayer = new ANNO_LAYER();
        m_AnnoLayer_map[pZNMapLayer->mNumber] = lpAnnoLayer;
    }
    // 在同一个layer中，可能同时存在text和sign两种类型
    if ( pZNMapLayer->mMapDataRep->m_textShapes.size() > 0 )
    {
        if ( !TransTextShape( lpAnnoLayer, pZNMapLayer ) )
        {
            return FALSE;
        }
    }
    if ( pZNMapLayer->mMapDataRep->m_signImageShapes.size() > 0 || 
        pZNMapLayer->mMapDataRep->m_signTextShapes.size() > 0 )
    {
        if ( !TransSignShape( lpAnnoLayer, pZNMapLayer ) )
        {
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CParseMapData::TransTextShape( PANNO_LAYER lpAnnoLayer, ZNMapLayer* pZNMapLayer )
{
    for ( CP_VECTOR<ZNMapTextShape*>::iterator it_text=pZNMapLayer->mMapDataRep->m_textShapes.begin(); 
        it_text!=pZNMapLayer->mMapDataRep->m_textShapes.end(); it_text++ )
    {
        ZNMapTextShape* pZNMapTextShape = *it_text;
        if ( pZNMapTextShape == NULL )
        {
            continue;
        }
        if ( pZNMapTextShape->m_textArray.size() <= 0 )
        {
            continue;
        }
        wstring wstr = pZNMapTextShape->m_textArray[0];
        if ( wstr.size() < 2 )
        {
            continue;
        }
        PANNO_FEATURE lpAnnoFeature = new ANNO_FEATURE();
        lpAnnoFeature->m_pText = new TCHAR [wstr.size()+1];
        memset( lpAnnoFeature->m_pText, 0, sizeof(TCHAR)*(wstr.size()+1) );
        _tcscpy( lpAnnoFeature->m_pText, wstr.c_str() );
        lpAnnoFeature->m_ptShape.x = pZNMapTextShape->m_llPoint.x;
        lpAnnoFeature->m_ptShape.y = pZNMapTextShape->m_llPoint.y;
        lpAnnoFeature->m_nKind = pZNMapTextShape->m_textData.mID;
        lpAnnoLayer->features.push_back( lpAnnoFeature );
    }

    return TRUE;
}

BOOL CParseMapData::TransLineShape( PBACK_LAYER lpBackLayer, ZNMapLayer* pZNMapLayer )
{
    for ( CP_VECTOR<ZNMapLineShape*>::iterator it_lps=pZNMapLayer->mMapDataRep->m_lineShapes.begin();
        it_lps!=pZNMapLayer->mMapDataRep->m_lineShapes.end(); it_lps++ )
    {
        ZNMapLineShape* pLineShape = *it_lps;
        if ( pLineShape == NULL || pLineShape->m_ZNMapLineAry.size() <= 0 )
        {
            continue;
        }
        for ( CP_VECTOR<ZNMapLine*>::iterator it_line=pLineShape->m_ZNMapLineAry.begin(); 
            it_line!=pLineShape->m_ZNMapLineAry.end(); it_line++ )
        {
            ZNMapLine* lpLine = *it_line;
            if ( lpLine == NULL )
            {
                continue;
            }
            PBACK_FEATURE lpBackFeature = new BACK_FEATURE();
            lpBackFeature->m_wptCount = lpLine->pointary.size();
            lpBackFeature->m_pShape = new GEOPOINT[lpBackFeature->m_wptCount];
            lpBackFeature->m_rcFeatureRect.dMinX = 180;
            lpBackFeature->m_rcFeatureRect.dMinY = 90;
            lpBackFeature->m_rcFeatureRect.dMaxX = 0;
            lpBackFeature->m_rcFeatureRect.dMaxY = 0;
            for ( int k=0; k<lpBackFeature->m_wptCount; k++ )
            {
                lpBackFeature->m_pShape[k].x = lpLine->pointary[k]->lon;
                lpBackFeature->m_pShape[k].y = lpLine->pointary[k]->lat;

                if ( lpBackFeature->m_pShape[k].x < lpBackFeature->m_rcFeatureRect.dMinX )
                {
                    lpBackFeature->m_rcFeatureRect.dMinX = lpBackFeature->m_pShape[k].x;
                }
                if ( lpBackFeature->m_pShape[k].x > lpBackFeature->m_rcFeatureRect.dMaxX )
                {
                    lpBackFeature->m_rcFeatureRect.dMaxX = lpBackFeature->m_pShape[k].x;
                }
                if ( lpBackFeature->m_pShape[k].y < lpBackFeature->m_rcFeatureRect.dMinY )
                {
                    lpBackFeature->m_rcFeatureRect.dMinY = lpBackFeature->m_pShape[k].y;
                }
                if ( lpBackFeature->m_pShape[k].y > lpBackFeature->m_rcFeatureRect.dMaxY )
                {
                    lpBackFeature->m_rcFeatureRect.dMaxY = lpBackFeature->m_pShape[k].y;
                }
            }
            lpBackLayer->features.push_back( lpBackFeature );
        }
    }

    return TRUE;
}

BOOL CParseMapData::TransLineShape( PROAD_LAYER lpRoadLayer, ZNMapLayer* pZNMapLayer )
{
    for ( CP_VECTOR<ZNMapLineShape*>::iterator it_lps=pZNMapLayer->mMapDataRep->m_lineShapes.begin();
        it_lps!=pZNMapLayer->mMapDataRep->m_lineShapes.end(); it_lps++ )
    {
        ZNMapLineShape* pLineShape = *it_lps;
        if ( pLineShape == NULL || pLineShape->m_ZNMapLineAry.size() <= 0 )
        {
            continue;
        }
        // 查询道路名称
        wstring wstrRoadName;
        map<int,wstring>::iterator it_rname = m_RoadName_map.find( pLineShape->mID );
        if ( it_rname != m_RoadName_map.end() )
        {
            wstrRoadName = it_rname->second;
        }

        // 一条道路可能会被打断为多条，则需要采用多个PROAD_FEATURE对象来处理
        // 但都具有同样的路名
        for ( CP_VECTOR<ZNMapLine*>::iterator it_line=pLineShape->m_ZNMapLineAry.begin(); 
            it_line!=pLineShape->m_ZNMapLineAry.end(); it_line++ )
        {
            ZNMapLine* lpLine = *it_line;
            if ( lpLine == NULL )
            {
                continue;
            }
            PROAD_FEATURE lpRoadFeature = new ROAD_FEATURE();
            if ( wstrRoadName.size() > 1 )
            {
                lpRoadFeature->m_pSzName = new TCHAR [wstrRoadName.size()+1];
                memset( lpRoadFeature->m_pSzName, 0, sizeof(TCHAR)*(wstrRoadName.size()+1) );
                _tcscpy( lpRoadFeature->m_pSzName, wstrRoadName.c_str() );
            }
            lpRoadFeature->m_wptCount = lpLine->pointary.size();
            lpRoadFeature->m_pShape = new GEOPOINT[lpRoadFeature->m_wptCount];
            lpRoadFeature->m_rcFeatureRect.dMinX = 180;
            lpRoadFeature->m_rcFeatureRect.dMinY = 90;
            lpRoadFeature->m_rcFeatureRect.dMaxX = 0;
            lpRoadFeature->m_rcFeatureRect.dMaxY = 0;
            for ( int k=0; k<lpRoadFeature->m_wptCount; k++ )
            {
                lpRoadFeature->m_pShape[k].x = lpLine->pointary[k]->lon;
                lpRoadFeature->m_pShape[k].y = lpLine->pointary[k]->lat;

                if ( lpRoadFeature->m_pShape[k].x < lpRoadFeature->m_rcFeatureRect.dMinX )
                {
                    lpRoadFeature->m_rcFeatureRect.dMinX = lpRoadFeature->m_pShape[k].x;
                }
                if ( lpRoadFeature->m_pShape[k].x > lpRoadFeature->m_rcFeatureRect.dMaxX )
                {
                    lpRoadFeature->m_rcFeatureRect.dMaxX = lpRoadFeature->m_pShape[k].x;
                }
                if ( lpRoadFeature->m_pShape[k].y < lpRoadFeature->m_rcFeatureRect.dMinY )
                {
                    lpRoadFeature->m_rcFeatureRect.dMinY = lpRoadFeature->m_pShape[k].y;
                }
                if ( lpRoadFeature->m_pShape[k].y > lpRoadFeature->m_rcFeatureRect.dMaxY )
                {
                    lpRoadFeature->m_rcFeatureRect.dMaxY = lpRoadFeature->m_pShape[k].y;
                }
            }
            lpRoadLayer->features.push_back( lpRoadFeature );
        }
    }

    return TRUE;
}

BOOL CParseMapData::TransPolygonShape( PBACK_LAYER lpBackLayer, ZNMapLayer* pZNMapLayer )
{
    if ( pZNMapLayer->mMapDataRep->m_polygonShapes.size() > 0 )
    {
        for ( CP_VECTOR<ZNMapPolygonShape1*>::iterator it=pZNMapLayer->mMapDataRep->m_polygonShapes.begin(); 
            it!=pZNMapLayer->mMapDataRep->m_polygonShapes.end(); it++ )
        {
            ZNMapPolygonShape1* lpShape = *it;
            if ( lpShape == NULL )
            {
                continue;
            }
            ZNFloatSection* lpPntList = NULL;
            if ( lpShape->mLLPointList.num > 0 )
            {
                lpPntList = &lpShape->mLLPointList;
                PBACK_FEATURE lpBackFeature = new BACK_FEATURE();
                if ( !GetPolygonShape( lpPntList, lpBackFeature ) )
                {
                    delete lpBackFeature;
                    lpBackFeature = NULL;
                }
                else
                {
                    lpBackLayer->features.push_back( lpBackFeature );
                }
            }
            else if ( lpShape->mLLPolygonPoint.size() > 0 )
            {
                for ( CP_VECTOR<ZNFloatSection*>::iterator it_sect=lpShape->mLLPolygonPoint.begin(); 
                    it_sect!=lpShape->mLLPolygonPoint.end(); it_sect++ )
                {
                    lpPntList = *it_sect;
                    PBACK_FEATURE lpBackFeature = new BACK_FEATURE();
                    if ( !GetPolygonShape( lpPntList, lpBackFeature ) )
                    {
                        delete lpBackFeature;
                        lpBackFeature = NULL;
                    }
                    else
                    {
                        lpBackLayer->features.push_back( lpBackFeature );
                    }
                }
            }
        }
    }

    if ( pZNMapLayer->mMapDataRep->m_polygonShape2s.size() > 0 )
    {
        for ( CP_VECTOR<ZNMapPolygonShape2*>::iterator it=pZNMapLayer->mMapDataRep->m_polygonShape2s.begin(); 
            it!=pZNMapLayer->mMapDataRep->m_polygonShape2s.end(); it++ )
        {
            ZNMapPolygonShape2* lpShape = *it;
            if ( lpShape == NULL )
            {
                continue;
            }
            ZNFloatSection* lpPntList = NULL;
            if ( lpShape->mLLPointList.num > 0 )
            {
                lpPntList = &lpShape->mLLPointList;
                PBACK_FEATURE lpBackFeature = new BACK_FEATURE();
                if ( !GetPolygonShape( lpPntList, lpBackFeature ) )
                {
                    delete lpBackFeature;
                    lpBackFeature = NULL;
                }
                else
                {
                    lpBackLayer->features.push_back( lpBackFeature );
                }
            }
            else if ( lpShape->mLLPolygonPoint.size() > 0 )
            {
                for ( CP_VECTOR<ZNFloatSection*>::iterator it_sect=lpShape->mLLPolygonPoint.begin(); 
                    it_sect!=lpShape->mLLPolygonPoint.end(); it_sect++ )
                {
                    lpPntList = *it_sect;
                    PBACK_FEATURE lpBackFeature = new BACK_FEATURE();
                    if ( !GetPolygonShape( lpPntList, lpBackFeature ) )
                    {
                        delete lpBackFeature;
                        lpBackFeature = NULL;
                    }
                    else
                    {
                        lpBackLayer->features.push_back( lpBackFeature );
                    }
                }
            }
        }
    }

    if ( pZNMapLayer->mMapDataRep->m_polygonShape3s.size() > 0 )
    {
        for ( CP_VECTOR<ZNMapPolygonShape3*>::iterator it=pZNMapLayer->mMapDataRep->m_polygonShape3s.begin(); 
            it!=pZNMapLayer->mMapDataRep->m_polygonShape3s.end(); it++ )
        {
            ZNMapPolygonShape3* lpShape = *it;
            if ( lpShape == NULL )
            {
                continue;
            }
            ZNFloatSection* lpPntList = NULL;
            if ( lpShape->mLLPointList.num > 0 )
            {
                lpPntList = &lpShape->mLLPointList;
                PBACK_FEATURE lpBackFeature = new BACK_FEATURE();
                if ( !GetPolygonShape( lpPntList, lpBackFeature ) )
                {
                    delete lpBackFeature;
                    lpBackFeature = NULL;
                }
                else
                {
                    lpBackLayer->features.push_back( lpBackFeature );
                }
            }
            else if ( lpShape->mLLPolygonPoint.size() > 0 )
            {
                for ( CP_VECTOR<ZNFloatSection*>::iterator it_sect=lpShape->mLLPolygonPoint.begin(); 
                    it_sect!=lpShape->mLLPolygonPoint.end(); it_sect++ )
                {
                    lpPntList = *it_sect;
                    PBACK_FEATURE lpBackFeature = new BACK_FEATURE();
                    if ( !GetPolygonShape( lpPntList, lpBackFeature ) )
                    {
                        delete lpBackFeature;
                        lpBackFeature = NULL;
                    }
                    else
                    {
                        lpBackLayer->features.push_back( lpBackFeature );
                    }
                }
            }
        }
    }

    if ( pZNMapLayer->mMapDataRep->m_polygonShape4s.size() > 0 )
    {
        for ( CP_VECTOR<ZNMapPolygonShape4*>::iterator it=pZNMapLayer->mMapDataRep->m_polygonShape4s.begin(); 
            it!=pZNMapLayer->mMapDataRep->m_polygonShape4s.end(); it++ )
        {
            ZNMapPolygonShape4* lpShape = *it;
            if ( lpShape == NULL )
            {
                continue;
            }
            ZNFloatSection* lpPntList = NULL;
            if ( lpShape->mLLPolygonPoint.size() > 0 )
            {
                for ( CP_VECTOR<ZNFloatSection*>::iterator it_sect=lpShape->mLLPolygonPoint.begin(); 
                    it_sect!=lpShape->mLLPolygonPoint.end(); it_sect++ )
                {
                    lpPntList = *it_sect;
                    PBACK_FEATURE lpBackFeature = new BACK_FEATURE();
                    if ( !GetPolygonShape( lpPntList, lpBackFeature ) )
                    {
                        delete lpBackFeature;
                        lpBackFeature = NULL;
                    }
                    else
                    {
                        lpBackLayer->features.push_back( lpBackFeature );
                    }
                }
            }
        }
    }

    if ( pZNMapLayer->mMapDataRep->m_polygonShape5s.size() > 0 )
    {
        for ( CP_VECTOR<ZNMapPolygonShape5*>::iterator it=pZNMapLayer->mMapDataRep->m_polygonShape5s.begin(); 
            it!=pZNMapLayer->mMapDataRep->m_polygonShape5s.end(); it++ )
        {
            ZNMapPolygonShape5* lpShape = *it;
            if ( lpShape == NULL )
            {
                continue;
            }
            ZNFloatSection* lpPntList = NULL;
            if ( lpShape->mLLPointList.num > 0 )
            {
                lpPntList = &lpShape->mLLPointList;
                PBACK_FEATURE lpBackFeature = new BACK_FEATURE();
                if ( !GetPolygonShape( lpPntList, lpBackFeature ) )
                {
                    delete lpBackFeature;
                    lpBackFeature = NULL;
                }
                else
                {
                    lpBackLayer->features.push_back( lpBackFeature );
                }
            }
            else if ( lpShape->mLLPolygonPoint.size() > 0 )
            {
                for ( CP_VECTOR<ZNFloatSection*>::iterator it_sect=lpShape->mLLPolygonPoint.begin(); 
                    it_sect!=lpShape->mLLPolygonPoint.end(); it_sect++ )
                {
                    lpPntList = *it_sect;
                    PBACK_FEATURE lpBackFeature = new BACK_FEATURE();
                    if ( !GetPolygonShape( lpPntList, lpBackFeature ) )
                    {
                        delete lpBackFeature;
                        lpBackFeature = NULL;
                    }
                    else
                    {
                        lpBackLayer->features.push_back( lpBackFeature );
                    }
                }
            }
        }
    }

    if ( pZNMapLayer->mMapDataRep->m_polygonShape6s.size() > 0 )
    {
        for ( CP_VECTOR<ZNMapPolygonShape6*>::iterator it=pZNMapLayer->mMapDataRep->m_polygonShape6s.begin(); 
            it!=pZNMapLayer->mMapDataRep->m_polygonShape6s.end(); it++ )
        {
            ZNMapPolygonShape6* lpShape = *it;
            if ( lpShape == NULL )
            {
                continue;
            }
            ZNFloatSection* lpPntList = NULL;
            if ( lpShape->mLLPointList.num > 0 )
            {
                lpPntList = &lpShape->mLLPointList;
                PBACK_FEATURE lpBackFeature = new BACK_FEATURE();
                if ( !GetPolygonShape( lpPntList, lpBackFeature ) )
                {
                    delete lpBackFeature;
                    lpBackFeature = NULL;
                }
                else
                {
                    lpBackLayer->features.push_back( lpBackFeature );
                }
            }
            else if ( lpShape->mLLPolygonPoint.size() > 0 )
            {
                for ( CP_VECTOR<ZNFloatSection*>::iterator it_sect=lpShape->mLLPolygonPoint.begin(); 
                    it_sect!=lpShape->mLLPolygonPoint.end(); it_sect++ )
                {
                    lpPntList = *it_sect;
                    PBACK_FEATURE lpBackFeature = new BACK_FEATURE();
                    if ( !GetPolygonShape( lpPntList, lpBackFeature ) )
                    {
                        delete lpBackFeature;
                        lpBackFeature = NULL;
                    }
                    else
                    {
                        lpBackLayer->features.push_back( lpBackFeature );
                    }
                }
            }
        }
    }

    return TRUE;
}

BOOL CParseMapData::TransSignShape( PANNO_LAYER lpAnnoLayer, ZNMapLayer* pZNMapLayer )
{
    // 只有图标，没有文字
    if ( pZNMapLayer->mMapDataRep->m_signImageShapes.size() > 0 )
    {
        for ( CP_VECTOR<ZNMapSignImageShape*>::iterator it=pZNMapLayer->mMapDataRep->m_signImageShapes.begin(); 
            it!=pZNMapLayer->mMapDataRep->m_signImageShapes.end(); it++ )
        {
            ZNMapSignImageShape* lpShape = *it;
            if ( lpShape == NULL )
            {
                continue;
            }
            PANNO_FEATURE lpAnnoFeature = new ANNO_FEATURE();
            lpAnnoFeature->m_ptShape.x = lpShape->mLLCenterOffset.x;
            lpAnnoFeature->m_ptShape.y = lpShape->mLLCenterOffset.y;
            lpAnnoFeature->m_nKind = lpShape->mSignData.mID;
            lpAnnoFeature->m_nSubKind = lpShape->mSignData.mNumber;
            lpAnnoLayer->features.push_back( lpAnnoFeature );
        }
    }

    if ( pZNMapLayer->mMapDataRep->m_signTextShapes.size() > 0 )
    {
        for ( CP_VECTOR<ZNMapSignTextShape*>::iterator it=pZNMapLayer->mMapDataRep->m_signTextShapes.begin(); 
            it!=pZNMapLayer->mMapDataRep->m_signTextShapes.end(); it++ )
        {
            ZNMapSignTextShape* lpShape = *it;
            if ( lpShape == NULL )
            {
                continue;
            }
            wstring wstr;
            if ( lpShape->mFlag == kZNMapShapeKind_Sign2 )
            {
                WCHAR szDigit[10] = {0};
                swprintf( szDigit, L"%d", lpShape->m_mapSignDigitData.mDigit );
                wstr = szDigit;
            }
            else if ( lpShape->mFlag == kZNMapShapeKind_Sign3 )
            {
                if ( lpShape->m_textArray.size() <= 0 )
                {
                    continue;
                }
                wstr = lpShape->m_textArray[0];
            }
            else if ( lpShape->mFlag == kZNMapShapeKind_Sign4 )
            {
                if ( lpShape->m_textArray.size() <= 0 )
                {
                    continue;
                }
                wstr = lpShape->m_textArray[0];
            }
            PANNO_FEATURE lpAnnoFeature = new ANNO_FEATURE();
            lpAnnoFeature->m_ptShape.x = lpShape->mLLPoint.x;
            lpAnnoFeature->m_ptShape.y = lpShape->mLLPoint.y;
            lpAnnoFeature->m_pText = new TCHAR [wstr.size()+1];
            memset( lpAnnoFeature->m_pText, 0, sizeof(TCHAR)*(wstr.size()+1) );
            _tcscpy( lpAnnoFeature->m_pText, wstr.c_str() );
            lpAnnoFeature->m_nKind = lpShape->m_mapSignTextData.mID;
            lpAnnoFeature->m_nSubKind = lpShape->m_mapSignTextData.mNumber;
            lpAnnoLayer->features.push_back( lpAnnoFeature );
        }
    }

    return TRUE;
}

BOOL CParseMapData::GetPolygonShape( ZNFloatSection* lpPntList, PBACK_FEATURE lpBackFeature )
{
    if ( lpPntList == NULL || lpBackFeature == NULL )
    {
        return FALSE;
    }
    lpBackFeature->m_wptCount = lpPntList->num;
    lpBackFeature->m_pShape = new GEOPOINT[lpBackFeature->m_wptCount];
    lpBackFeature->m_rcFeatureRect.dMinX = 180;
    lpBackFeature->m_rcFeatureRect.dMinY = 90;
    lpBackFeature->m_rcFeatureRect.dMaxX = 0;
    lpBackFeature->m_rcFeatureRect.dMaxY = 0;
    for ( int k=0; k<lpBackFeature->m_wptCount; k++ )
    {
        lpBackFeature->m_pShape[k].x = lpPntList->array[2*k];
        lpBackFeature->m_pShape[k].y = lpPntList->array[2*k+1];

        if ( lpBackFeature->m_pShape[k].x < lpBackFeature->m_rcFeatureRect.dMinX )
        {
            lpBackFeature->m_rcFeatureRect.dMinX = lpBackFeature->m_pShape[k].x;
        }
        if ( lpBackFeature->m_pShape[k].x > lpBackFeature->m_rcFeatureRect.dMaxX )
        {
            lpBackFeature->m_rcFeatureRect.dMaxX = lpBackFeature->m_pShape[k].x;
        }
        if ( lpBackFeature->m_pShape[k].y < lpBackFeature->m_rcFeatureRect.dMinY )
        {
            lpBackFeature->m_rcFeatureRect.dMinY = lpBackFeature->m_pShape[k].y;
        }
        if ( lpBackFeature->m_pShape[k].y > lpBackFeature->m_rcFeatureRect.dMaxY )
        {
            lpBackFeature->m_rcFeatureRect.dMaxY = lpBackFeature->m_pShape[k].y;
        }
    }
    return TRUE;
}

PROAD_LAYER CParseMapData::CloneRoadLayer( int nCloneID )
{
    PROAD_LAYER lplayer = NULL;
    map<int,PROAD_LAYER>::iterator it_layer = m_RoadLayer_map.find( nCloneID );
    if ( it_layer != m_RoadLayer_map.end() )
    {
        lplayer = it_layer->second;
    }
    if ( lplayer == NULL )
    {
        return NULL;
    }
    PROAD_LAYER lpCloneLayer = new ROAD_LAYER();
    for ( vector<PROAD_FEATURE>::iterator it=lplayer->features.begin(); it!=lplayer->features.end(); it++ )
    {
        PROAD_FEATURE lpfeature = *it;

        PROAD_FEATURE lpRoadFeature = new ROAD_FEATURE();
        lpRoadFeature->m_rcFeatureRect = lpfeature->m_rcFeatureRect;
        lpRoadFeature->m_wptCount = lpfeature->m_wptCount;
        if ( lpfeature->m_pSzName != NULL )
        {
            lpRoadFeature->m_pSzName = new TCHAR [_tcslen(lpfeature->m_pSzName)+1];
            _tcscpy( lpRoadFeature->m_pSzName, lpfeature->m_pSzName );
        }
        lpRoadFeature->m_pShape = new GEOPOINT[lpRoadFeature->m_wptCount];
        memcpy( lpRoadFeature->m_pShape, lpfeature->m_pShape, lpRoadFeature->m_wptCount*sizeof(GEOPOINT) );

        lpCloneLayer->features.push_back( lpRoadFeature );
    }
    return lpCloneLayer;
}

PANNO_LAYER CParseMapData::CloneAnnoLayer( int nCloneID, int nKind, int nSubKind )
{
    if ( nKind == -1 )
    {
        return NULL;
    }
    PANNO_LAYER lplayer = NULL;
    map<int,PANNO_LAYER>::iterator it_layer = m_AnnoLayer_map.find( nCloneID );
    if ( it_layer != m_AnnoLayer_map.end() )
    {
        lplayer = it_layer->second;
    }
    if ( lplayer == NULL )
    {
        return NULL;
    }
    PANNO_LAYER lpCloneLayer = new ANNO_LAYER();
    for ( vector<PANNO_FEATURE>::iterator it=lplayer->features.begin(); it!=lplayer->features.end(); )
    {
        PANNO_FEATURE lpfeature = *it;
        if ( lpfeature->m_nKind != nKind || ( nSubKind != -1 && lpfeature->m_nSubKind != nSubKind ) )
        {
            it++;
            continue;
        }
        // 从原来的Layer中删除
        lpCloneLayer->features.push_back( lpfeature );
        it = lplayer->features.erase( it );
    }
    return lpCloneLayer;
}


