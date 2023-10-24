#pragma once
#include "DisplayInfo.h"
#include "LabelAdjust.h"


class SpecialLayerPointNameListLabel : public IPointNameListLabel
{
public:
    SpecialLayerPointNameListLabel();
    ~SpecialLayerPointNameListLabel();

    //的到列表大小
    virtual int Size();

    //得到一项的坐标
    virtual GEOPOINT* GetItemPoint(int index);

    //得到一项的注记值
    virtual TCHAR* GetItemText(int index);

    //得到点风格
    virtual CGeoPointStyle* GetPointStyle(int index);

    //得到注记风格
    virtual CGeoTextStyle* GetTextStyle(int index=0);

    //是否显示
    virtual BOOL GetDispState(int index);

    void SetLayer(PSPECIAL_LAYER pLyr);

protected:

    PSPECIAL_LAYER pLayer;
    CGeoPointStyle* m_pGeoPointStyle;
    CGeoTextStyle*  m_pGeoTextStyle;

private:

};


class CSpecialPointRender 
{
public:
    CSpecialPointRender();
    ~CSpecialPointRender();


    void Draw(CDisplayInfo* pDisplayInfo, PMAP_DISP_DATA pData, PTILETEXT_INFO pTileTextInfo);

protected:

    void DrawLayer(CDisplayInfo* pDisplayInfo,PSPECIAL_LAYER pLayer, PTILETEXT_INFO pTileTextInfo);


};