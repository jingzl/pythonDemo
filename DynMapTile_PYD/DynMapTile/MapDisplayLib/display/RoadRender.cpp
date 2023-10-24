#include "StdAfx.h"
#include "RoadRender.h"
#include "globalfun.h"



CRoadRender::CRoadRender(void)
{
    ZeroMemory( &m_rcClip, sizeof(RECT) );
    ZeroMemory( &m_minPt, sizeof(POINT) );
    ZeroMemory( &m_maxPt, sizeof(POINT) );
    ZeroMemory( m_LineList, 16*sizeof(int) );

	InitializeCriticalSection(&m_cs);
}

CRoadRender::~CRoadRender(void)
{
	DeleteCriticalSection(&m_cs);
}

void CRoadRender::Draw(CDisplayInfo* pDisplayInfo, PMAP_DISP_DATA pData)
{

	if(pDisplayInfo && pData)
	{
		//清除数据
		EnterCriticalSection(&m_cs);
		m_RoadLinks.Clear();
		LeaveCriticalSection(&m_cs);
		for (UINT i=0; i<pData->roadLayers.size(); i++)
		{
			DrawLayer(pDisplayInfo, pData->roadLayers[i]);
		}
	}
}

void CRoadRender::DrawName( CDisplayInfo* pDisplayInfo, PMAP_DISP_DATA pData )
{
    if ( pDisplayInfo == NULL || pData == NULL )
    {
        return;
    }
    if ( m_RoadLinks.RoadLinks->size() <= 0 )
    {
        return;
    }

    ROADLINKIT it = (*m_RoadLinks.RoadLinks).begin();
    vector<stLinkPart>* pLinkVct = &((*it).second);
    m_TextStyle.InitMe(&(*pLinkVct)[0].pLayer->textMode);
    m_TextStyle.InitStyle( pDisplayInfo->GetCanvas() );
	for ( ROADLINKIT it = (*m_RoadLinks.RoadLinks).begin(); it!=(*m_RoadLinks.RoadLinks).end(); ++it )
	{
		pLinkVct = &((*it).second);
        // 多层要素在一起，样式可能不一样
		m_TextStyle.InitMe(&(*pLinkVct)[0].pLayer->textMode);
		pDisplayInfo->GetLabelAdjust()->DrawLineName( pDisplayInfo, pDisplayInfo->GetCanvas(), (TCHAR*)(*it).first.c_str(), pLinkVct, &m_TextStyle );
	}
    m_TextStyle.UnInitStyle( pDisplayInfo->GetCanvas() );

}

//////////////////////////////////////////////////////////////////////////
// 
void CRoadRender::DrawLayer(CDisplayInfo* pDisplayInfo, PROAD_LAYER pLayer)
{
	if(pDisplayInfo && pLayer)
	{
		if(pLayer->lineMode.m_drawtype == eLINE_DT_DOUBLE)
		{
			m_PolylineStyle.InitMeBack(&pLayer->lineMode);
			m_PolylineStyle.InitStyle(pDisplayInfo->GetCanvas());
			for (UINT i = 0; i<pLayer->features.size(); i++)
			{
				DrawFeature(pDisplayInfo, pLayer, pLayer->features[i], m_PolylineStyle);
			}
			m_PolylineStyle.UnInitStyle(pDisplayInfo->GetCanvas());
		}

		m_PolylineStyle.InitMe(&pLayer->lineMode);
		m_PolylineStyle.InitStyle(pDisplayInfo->GetCanvas());
		for (UINT i = 0; i<pLayer->features.size(); i++)
		{
			DrawFeature(pDisplayInfo, pLayer, pLayer->features[i], m_PolylineStyle);
		}
		m_PolylineStyle.UnInitStyle(pDisplayInfo->GetCanvas());		
	}
}

void CRoadRender::DrawFeature(CDisplayInfo* pDisplayInfo, PROAD_LAYER pLayer, PROAD_FEATURE pFeature, CGeoPolylineStyle& style)
{
	GEOPOINT* pSrcpt = pFeature->m_pShape;
    POINT* pDespt = pDisplayInfo->GetPointOut();

	// 地图到显示区坐标转化
	for(int i=0; i<pFeature->m_wptCount; i++, pSrcpt++)
	{
		if(i>0)
		{
			*pDespt = pDisplayInfo->Map2Display(pSrcpt->x, pSrcpt->y);

			if(((pDespt-1)->x==pDespt->x)&&((pDespt-1)->y==pDespt->y))
				continue;
		}
		else
		{
			*pDespt = pDisplayInfo->Map2Display(pSrcpt->x, pSrcpt->y);
		}
		pDespt++;
	}
	int t = (pDespt-pDisplayInfo->GetPointOut());
	if ( t > 1 )
	{
		m_rcClip = pDisplayInfo->GetBufRect();
		m_minPt.x = m_rcClip.left;
		m_minPt.y = m_rcClip.top;
		m_maxPt.x = m_rcClip.right;
		m_maxPt.y = m_rcClip.bottom;
        ZeroMemory( m_LineList, 16 * sizeof(int) );
		int tc = RectClipLine(pDisplayInfo->GetPointOut(), t, pDisplayInfo->GetHalfPointOut(), m_LineList, &m_minPt, &m_maxPt );
		if ( tc >= 1 )
		{
			int pos = 0;
			for( int jj=0; jj<tc; jj++ )
			{
				CGeoRender::Polyline(pDisplayInfo->GetCanvas(), pDisplayInfo->GetHalfPointOut() + pos, m_LineList[jj]-pos, style);
				if(pFeature->m_pSzName && pLayer->textMode.m_nID > -1 && wcslen(pFeature->m_pSzName) > 0)
				{
					EnterCriticalSection(&m_cs);
					m_RoadLinks.AddLinkPart(pFeature->m_pSzName, pDisplayInfo->GetHalfPointOut() + pos, m_LineList[jj]-pos,pLayer);
					LeaveCriticalSection(&m_cs);
				}
				pos = m_LineList[jj];
			}
		}
	}
}

