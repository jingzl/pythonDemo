#pragma once
#include "globaldefine.h"




class CAttachDisplayMode 
{
public:
    CAttachDisplayMode();
    ~CAttachDisplayMode();

    // Init
    BOOL Init( const ST_POINTDISPLAYMODE_ARY* lpPointDisplayModeAry, const ST_LINEDISPLAYMODE_ARY* lpLineDisplayModeAry,
       const ST_AREADISPLAYMODE_ARY* lpAreaDisplayModeAry, const ST_TXTDISPLAYMODE_ARY* lpTxtDisplayModeAry );

    // ÃÌº”‰÷»æ
    BOOL AttachDisplayMode( LPST_LAYER_CFG lpLayerCfg, PBACK_LAYER lplayer );
    BOOL AttachDisplayMode( LPST_LAYER_CFG lpLayerCfg, PROAD_LAYER lplayer );
    BOOL AttachDisplayMode( LPST_LAYER_CFG lpLayerCfg, PANNO_LAYER lplayer );
    BOOL AttachDisplayMode( LPST_LAYER_CFG lpLayerCfg, PPOI_LAYER lplayer );

protected:

    // ªÒ»°‰÷»æ≈‰÷√
    BOOL QueryDisplayModeCfg( int nID, ST_POINTDISPLAYMODE& pointdisplaymode );
    BOOL QueryDisplayModeCfg( int nID, ST_LINEDISPLAYMODE& linedisplaymode );
    BOOL QueryDisplayModeCfg( int nID, ST_AREADISPLAYMODE& areadisplaymode );
    BOOL QueryDisplayModeCfg( int nID, ST_TXTDISPLAYMODE& txtdisplaymode );


private:
    map<int,LPST_POINTDISPLAYMODE> m_PointDisplayMode_map;
    map<int,LPST_LINEDISPLAYMODE> m_LineDisplayMode_map;
    map<int,LPST_AREADISPLAYMODE> m_AreaDisplayMode_map;
    map<int,LPST_TXTDISPLAYMODE> m_TxtDisplayMode_map;

};
