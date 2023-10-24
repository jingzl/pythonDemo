#include "stdafx.h"
#include "SpecialPointRender.h"



SpecialLayerPointNameListLabel::SpecialLayerPointNameListLabel()
{
    pLayer = NULL;
    m_pGeoPointStyle = NULL;
    m_pGeoTextStyle = NULL;
}

SpecialLayerPointNameListLabel::~SpecialLayerPointNameListLabel()
{
    pLayer = NULL;
    if(m_pGeoPointStyle)
    {
        delete m_pGeoPointStyle;
        m_pGeoPointStyle = NULL;
    }
    if(m_pGeoTextStyle)
    {
        delete m_pGeoTextStyle;
        m_pGeoTextStyle = NULL;
    }
}

int SpecialLayerPointNameListLabel::Size()
{
    if (pLayer == NULL)
    {
        return 0;
    }
    else
    {
        return pLayer->features.size();
    }
}

GEOPOINT* SpecialLayerPointNameListLabel::GetItemPoint(int index)
{
    if (pLayer == NULL)
    {
        return NULL;
    }
    else
    {
        if(index < pLayer->features.size() &&  index > -1)
        {
            return &(pLayer->features[index]->m_ptShape);
        }
        else
        {
            return NULL;
        }
    }
}

TCHAR* SpecialLayerPointNameListLabel::GetItemText(int index)
{
    if (pLayer == NULL)
    {
        return NULL;
    }
    else
    {
        if(index < pLayer->features.size() &&  index > -1)
        {
            return (pLayer->features[index]->m_pText);
        }
        else
        {
            return NULL;
        }
    }
}

CGeoPointStyle* SpecialLayerPointNameListLabel::GetPointStyle(int index)
{
    return m_pGeoPointStyle;
}

CGeoTextStyle* SpecialLayerPointNameListLabel::GetTextStyle(int index)
{
    return  m_pGeoTextStyle;
}
void SpecialLayerPointNameListLabel::SetLayer(PSPECIAL_LAYER pLyr)
{
    pLayer = pLyr;
    if(pLayer)
    {
        if(m_pGeoPointStyle == NULL)
        {
            m_pGeoPointStyle = new CGeoPointStyle();
        }
        m_pGeoPointStyle->InitMe(&pLayer->pointMode);


        if(m_pGeoTextStyle == NULL)
        {
            m_pGeoTextStyle = new CGeoTextStyle();
        }
        m_pGeoTextStyle->InitMe(&pLayer->textMode);
    }
}

BOOL SpecialLayerPointNameListLabel::GetDispState(int index)
{
    return TRUE;
}



CSpecialPointRender::CSpecialPointRender()
{
}

CSpecialPointRender::~CSpecialPointRender()
{
}

void CSpecialPointRender::Draw(CDisplayInfo* pDisplayInfo, PMAP_DISP_DATA pData, PTILETEXT_INFO pTileTextInfo)
{
    vector<PSPECIAL_LAYER>::iterator it_special;
    for ( it_special=pData->specialLayers.begin(); it_special!=pData->specialLayers.end(); it_special++ )
    {
        PSPECIAL_LAYER lpLayer = *it_special;
        if ( lpLayer != NULL )
        {
            DrawLayer(pDisplayInfo, lpLayer, pTileTextInfo);
        }
    }
}
void CSpecialPointRender::DrawLayer(CDisplayInfo* pDisplayInfo,PSPECIAL_LAYER pLayer, PTILETEXT_INFO pTileTextInfo)
{
    // 目前特殊层数据只能放在 注记数据 中，采用的注记数据的图标文件夹。
    SpecialLayerPointNameListLabel alpnll;
    alpnll.SetLayer(pLayer);
    if(alpnll.GetPointStyle(0))
    {
        alpnll.GetPointStyle(0)->InitStyle(pDisplayInfo->GetCanvas());
    }
    if(alpnll.GetTextStyle(0))
    {
        alpnll.GetTextStyle(0)->InitStyle(pDisplayInfo->GetCanvas());
    }
    pDisplayInfo->GetLabelAdjust()->DrawPointName( pDisplayInfo,pDisplayInfo->GetCanvas(), &alpnll, pTileTextInfo, DRAW_BOTH);
    if(alpnll.GetPointStyle(0))
    {
        alpnll.GetPointStyle(0)->UnInitStyle(pDisplayInfo->GetCanvas());
    }
    if(alpnll.GetTextStyle(0))
    {
        alpnll.GetTextStyle(0)->UnInitStyle(pDisplayInfo->GetCanvas());
    }
}

