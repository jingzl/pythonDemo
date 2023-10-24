#include "StdAfx.h"
#include "PoiRender.h"


POILayerPointNameListLabel::POILayerPointNameListLabel()
{
	m_pGeoPointStyle = NULL;
	m_pGeoTextStyle = NULL;

	m_pLayer = NULL;
}

POILayerPointNameListLabel::~POILayerPointNameListLabel()
{
	if(m_pGeoPointStyle != NULL)
	{
		delete m_pGeoPointStyle;
		m_pGeoPointStyle = NULL;
	}

	if(m_pGeoTextStyle != NULL)
	{
		delete m_pGeoTextStyle;
		m_pGeoTextStyle = NULL;
	}
}

int POILayerPointNameListLabel::Size()
{
	if ( m_pLayer != NULL )
	{
		return m_pLayer->features.size();
	}
    else
    {
        return 0;
    }
}

GEOPOINT* POILayerPointNameListLabel::GetItemPoint(int index)
{
    if ( index < 0 )
    {
        return NULL;
    }
    if ( m_pLayer != NULL )
    {
        if ( index > m_pLayer->features.size()-1 )
        {
            return NULL;
        }
        return &(m_pLayer->features[index]->m_ptShape);
    }
    return NULL;
}

TCHAR* POILayerPointNameListLabel::GetItemText(int index)
{
    if ( index < 0 )
    {
        return NULL;
    }

    if ( m_pLayer != NULL )
    {
        if ( index > m_pLayer->features.size()-1 )
        {
            return NULL;
        }
        return (m_pLayer->features[index]->m_pText);
    }
    return NULL;
}

CGeoPointStyle* POILayerPointNameListLabel::GetPointStyle(int index)
{
	return m_pGeoPointStyle;
}

CGeoTextStyle* POILayerPointNameListLabel::GetTextStyle(int index)
{
	return m_pGeoTextStyle;
}

BOOL POILayerPointNameListLabel::GetDispState(int index)
{
    if ( index < 0 )
    {
        return FALSE;
    }
    if ( m_pLayer != NULL )
    {
        if ( index > m_pLayer->features.size()-1 )
        {
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

void POILayerPointNameListLabel::SetLayer(PPOI_LAYER pLyr)
{
	m_pLayer = pLyr;
	if(m_pGeoPointStyle == NULL)
	{
		m_pGeoPointStyle = new CGeoPointStyle();
		TCHAR m_szPath[MAX_PATH];
		TCHAR path[MAX_PATH];
		if(GetBinPath(path) > 0)
		{
			wsprintfW(m_szPath, L"%s\\config\\symbol\\POICD", path);
		}
		m_pGeoPointStyle->SetImagePath(m_szPath);
		if (m_pLayer)
		{
			m_pGeoPointStyle->InitMe(&m_pLayer->pointMode);
		}
	}

	if(m_pGeoTextStyle == NULL)
	{
		m_pGeoTextStyle = new CGeoTextStyle();
		if (m_pLayer)
		{
			m_pGeoTextStyle->InitMe(&m_pLayer->textMode);
		}
	}
}



CPoiRender::CPoiRender(void)
{
}

CPoiRender::~CPoiRender(void)
{
}

void CPoiRender::Draw(CDisplayInfo* pDisplayInfo, PMAP_DISP_DATA pData, PTILETEXT_INFO pTileTextInfo)
{
	vector<PPOI_LAYER>::iterator it_poi;
	for ( it_poi=pData->poiLayers.begin(); it_poi!=pData->poiLayers.end(); it_poi++ )
	{
		PPOI_LAYER lpLayer = *it_poi;
		if ( lpLayer != NULL )
		{
			DrawLayer(pDisplayInfo, lpLayer, pTileTextInfo);
		}
	}
}

void CPoiRender::DrawLayer(CDisplayInfo* pDisplayInfo, PPOI_LAYER lpLayer, PTILETEXT_INFO pTileTextInfo)
{
    POILayerPointNameListLabel alpnll;
    alpnll.SetLayer(lpLayer);
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
