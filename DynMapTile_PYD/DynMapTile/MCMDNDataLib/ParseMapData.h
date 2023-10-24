#pragma once
#include "IParser.h"
#include "AttachDisplayMode.h"
#include "ZNMapDecoder.h"
#include "DataType.h"
#include "CPStl.h"
#include "MapTileCache.h"



class CParseMapData : public IParser 
{
public:
    static CParseMapData* Instance(void);
    ~CParseMapData(void);


    // 设置日志
    virtual void SetLogFunc( LogFunc lplogfunc );

    // 设置地图显示配置
    virtual void SetMapDisplayConfig( const ST_DISPLAYMODE_ARY* lpDisplayModeAry );

    // 设置显示样式配置
    virtual void SetDisplayModeConfig( const ST_POINTDISPLAYMODE_ARY* lpPointDisplayModeAry );
    virtual void SetDisplayModeConfig( const ST_LINEDISPLAYMODE_ARY* lpLineDisplayModeAry );
    virtual void SetDisplayModeConfig( const ST_AREADISPLAYMODE_ARY* lpAreaDisplayModeAry );
    virtual void SetDisplayModeConfig( const ST_TXTDISPLAYMODE_ARY* lpTxtDisplayModeAry );

    // 设置当前的地图比例尺等级配置
    virtual void SetMapScaleConfig( ST_SCALECFG_ARY* scalecfgary );

    // Init
    virtual BOOL Init( const char* lpszDataPath );

    // 获取数据
    virtual BOOL GetMapData( int nDisplayMode, BYTE nMapScaleIndex, GEORECT rect, PMAP_DISP_DATA& lpMapDispData );

    // 释放地图数据
    virtual void ReleaseMapData( PMAP_DISP_DATA lpMapDispData );

    // 释放
    virtual void Release();

protected:
    void OutputMessage( CString str );

    // Get Scale --> MLevel index
    int GetMapScaleIndex( int nScale );

    // Get GridRect Data
    BOOL GetGridRectData( GRIDRECT_LAYER& gridrectlayer );
    BOOL GetMCMDNRect( GRIDRECT_LAYER& gridrectlayer );
    BOOL GetGoogleTileRect( GRIDRECT_LAYER& gridrectlayer );

    // Get MCMDN data
    BOOL GetMCMDNData( GRIDRECT_LAYER& gridrectlayer, CP_VECTOR<ZNMapTile*>& mapTileAry );
    // Trans to Display Data
    BOOL Trans2DisplayData( LPST_SCALELEVEL lpLevel, CP_VECTOR<ZNMapTile*>& mapTileAry, PMAP_DISP_DATA& lpMapDispData );

    // extract road name
    BOOL ExtractRoadName( CP_VECTOR<ZNMapTile*>& mapTileAry );

    BOOL TransBK( LPST_LAYER_CFG lpLayerCfg, ZNMapLayer* pZNMapLayer );
    BOOL TransRoad( LPST_LAYER_CFG lpLayerCfg, ZNMapLayer* pZNMapLayer );
    BOOL TransAnno( LPST_LAYER_CFG lpLayerCfg, ZNMapLayer* pZNMapLayer );

    BOOL TransTextShape( PANNO_LAYER lpAnnoLayer, ZNMapLayer* pZNMapLayer );
    BOOL TransLineShape( PBACK_LAYER lpBackLayer, ZNMapLayer* pZNMapLayer );
    BOOL TransLineShape( PROAD_LAYER lpRoadLayer, ZNMapLayer* pZNMapLayer );
    BOOL TransPolygonShape( PBACK_LAYER lpBackLayer, ZNMapLayer* pZNMapLayer );
    BOOL TransSignShape( PANNO_LAYER lpAnnoLayer, ZNMapLayer* pZNMapLayer );

    BOOL GetPolygonShape( ZNFloatSection* lpPntList, PBACK_FEATURE lpBackFeature );

    PROAD_LAYER CloneRoadLayer( int nCloneID );
    PANNO_LAYER CloneAnnoLayer( int nCloneID, int nKind, int nSubKind );


private:
    LogFunc m_lpLogFunc;
    CString m_strMsg;

    char m_szDataPath[MAX_PATH];
    BOOL m_bInit;

    // 数据配置
    const ST_DISPLAYMODE_ARY* m_lpDisplayModeAry;
    const ST_POINTDISPLAYMODE_ARY* m_lpPointDispModeAry;
    const ST_LINEDISPLAYMODE_ARY* m_lpLineDispModeAry;
    const ST_AREADISPLAYMODE_ARY* m_lpAreaDispModeAry;
    const ST_TXTDISPLAYMODE_ARY* m_lpTxtDispModeAry;
    ST_SCALECFG_ARY* m_lpScaleCfgAry;

    // 获取数据的参数
    int m_nMapScale;
    int m_nMapScaleIndex;
    GEORECT m_GeoRect;

    // 要素显示样式匹配对象
    CAttachDisplayMode m_AttachDisplayMode;

    map<int, PBACK_LAYER> m_BackLayer_map;
    map<int, PROAD_LAYER> m_RoadLayer_map;
    map<int, PANNO_LAYER> m_AnnoLayer_map;

    // 道路名称索引
    vector<int> m_RoadNameLayerID_ary;
    map<int, wstring> m_RoadName_map;

    // Tile Cache
    CMapTileCache m_MapTileCache;

private:
    CParseMapData(void);
    static CParseMapData* m_sinstance;

};

IParser* GetParserInstance()
{
    return CParseMapData::Instance();
}
