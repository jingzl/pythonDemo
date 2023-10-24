#include "StdAfx.h"
#include "BackRender.h"

CBackRender::CBackRender(void)
{
}

CBackRender::~CBackRender(void)
{
}

void CBackRender::Draw(CDisplayInfo* pDisplayInfo, PMAP_DISP_DATA pData)
{
	if(pDisplayInfo && pData)
	{
		for (UINT i=0; i<pData->backLayers.size(); i++)
		{
			DrawLayer(pDisplayInfo, pData->backLayers[i]);
		}
	}
}

void CBackRender::DrawGridRect(CDisplayInfo* pDisplayInfo, PGRIDRECT_LAYER pGridRectLayer)
{
    if(pDisplayInfo && pGridRectLayer)
    {
        m_PolylineStyle.InitMe(&pGridRectLayer->lineMode);
        m_PolylineStyle.InitStyle(pDisplayInfo->GetCanvas());
        for (UINT i = 0; i<pGridRectLayer->features.size(); i++)
        {
            DrawLineFeature(pDisplayInfo, pGridRectLayer->features[i], m_PolylineStyle);
        }
        m_PolylineStyle.UnInitStyle(pDisplayInfo->GetCanvas());
    }
}

//////////////////////////////////////////////////////////////////////////
// 
void CBackRender::DrawLayer(CDisplayInfo* pDisplayInfo,PBACK_LAYER pLayer)
{
	if(pDisplayInfo && pLayer)
	{
		if(pLayer->type == eGeoLine)
		{			
			if(pLayer->lineMode.m_drawtype == eLINE_DT_DOUBLE)
			{
				m_PolylineStyle.InitMeBack(&pLayer->lineMode);
				m_PolylineStyle.InitStyle(pDisplayInfo->GetCanvas());
				for (UINT i = 0; i<pLayer->features.size(); i++)
				{
					DrawLineFeature(pDisplayInfo, pLayer->features[i], m_PolylineStyle);
				}
				m_PolylineStyle.UnInitStyle(pDisplayInfo->GetCanvas());
			}

			m_PolylineStyle.InitMe(&pLayer->lineMode);
			m_PolylineStyle.InitStyle(pDisplayInfo->GetCanvas());
			for (UINT i = 0; i<pLayer->features.size(); i++)
			{
				DrawLineFeature(pDisplayInfo, pLayer->features[i], m_PolylineStyle);
			}
			m_PolylineStyle.UnInitStyle(pDisplayInfo->GetCanvas());
		}
		else if(pLayer->type = eGeoArea)
		{
			m_PolygonStyle.InitMe(&pLayer->areaMode);
			m_PolygonStyle.InitStyle(pDisplayInfo->GetCanvas());
			for (UINT i = 0; i<pLayer->features.size(); i++)
			{
				DrawAreaFeature(pDisplayInfo, pLayer->features[i], m_PolygonStyle);
			}
			m_PolygonStyle.UnInitStyle(pDisplayInfo->GetCanvas());
		}
	}
}

void CBackRender::DrawAreaFeature(CDisplayInfo* pDisplayInfo, PBACK_FEATURE pFeature, CGeoPolygonStyle& style)
{
    // 没有裁剪
	GEOPOINT* pSrcpt = pFeature->m_pShape;
	POINT* pDespt = pDisplayInfo->GetPointOut();
	GEOPOINT ptLast;

	//地图到显示区坐标转化
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
		ptLast.x = pSrcpt->x;
		ptLast.y = pSrcpt->y;
	}
	int t = (pDespt-pDisplayInfo->GetPointOut());
	if(t>1)
	{
		CGeoRender::Polygon(
			pDisplayInfo->GetCanvas(),
			pDisplayInfo->GetPointOut(), 
			t, style);
	}

    /*
    // 使用裁剪，发现Bug
    //是否在窗口区
    RECT rcClip1 = pDisplayInfo->GetBufRect();

    CCanvas* pCanvas = pDisplayInfo->GetCanvas();
    GEOPOINT* pSrcpt = pFeature->m_pShape;
    POINT* pDespt = pDisplayInfo->GetPointOut();
    POINT* pDespt2 = pDespt;

    //地图到显示区坐标转化
    *pDespt = pDisplayInfo->Map2Display(pSrcpt->x, pSrcpt->y);
    pSrcpt++;
    pDespt++;
    for ( int i=1; i<pFeature->m_wptCount; i++, pSrcpt++ )
    {
        *pDespt = pDisplayInfo->Map2Display(pSrcpt->x, pSrcpt->y);
        if(((pDespt-1)->x==pDespt->x)&&((pDespt-1)->y==pDespt->y))
        {
            continue;
        }
        pDespt++;
    }
    int t = ( pDespt - pDespt2 );
    if ( t > 2 )
    {
        int tc = clipGraph( &rcClip1, pDespt2, t, pDespt2+10240, 0 );
        if ( tc > 2 )
        {
            int t1 = clipGraph(&rcClip1, pDespt2+10240, tc, pDespt2+15360, 0);
            if(t1>2)
            {
                CGeoRender::Polygon(pDisplayInfo->GetCanvas(),pDespt2+15360, t1,style);
            }
        }
    }
    */
}

void CBackRender::DrawLineFeature(CDisplayInfo* pDisplayInfo, PBACK_FEATURE pFeature, CGeoPolylineStyle& style)
{
    GEOPOINT* pSrcpt = pFeature->m_pShape;
    POINT* pDespt = pDisplayInfo->GetPointOut();
    GEOPOINT ptLast;

    //地图到显示区坐标转化
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
        ptLast.x = pSrcpt->x;
        ptLast.y = pSrcpt->y;
    }
    int t = (pDespt-pDisplayInfo->GetPointOut());
    if(t>1)
    {
        CGeoRender::Polyline(
            pDisplayInfo->GetCanvas(),
            pDisplayInfo->GetPointOut(), 
            t, style);
    }

    /*
    // 裁剪算法有Bug。
	GEOPOINT* pSrcpt = pFeature->m_pShape;
	POINT* pDespt = pDisplayInfo->GetPointOut();
    POINT* pDespt2 = pDespt;
    RECT rcClip1;
    rcClip1 = pDisplayInfo->GetBufRect();
	GEOPOINT ptLast;

	//地图到显示区坐标转化
    *pDespt = pDisplayInfo->Map2Display(pSrcpt->x, pSrcpt->y);
    pSrcpt++;
    pDespt++;
	for(int i=1; i<pFeature->m_wptCount; i++, pSrcpt++)
	{
		*pDespt = pDisplayInfo->Map2Display(pSrcpt->x, pSrcpt->y);
		if(((pDespt-1)->x==pDespt->x)&&((pDespt-1)->y==pDespt->y))
        {
			continue;
        }
		pDespt++;
	}
	int t = (pDespt-pDespt2);
    if ( t >= 2 )
    {
        int tc = clipGraph( &rcClip1, pDespt2, t, pDespt2+t, TRUE );
        if(tc>=2)
        {
            CGeoRender::Polyline( pDisplayInfo->GetCanvas(), pDespt2+t, tc, style );
        }
    }
    */

}

void CBackRender::DrawLineFeature(CDisplayInfo* pDisplayInfo, PGRIDRECT_FEATURE pFeature, CGeoPolylineStyle& style)
{
    GEOPOINT* pSrcpt = pFeature->m_pShape;
    POINT* pDespt = pDisplayInfo->GetPointOut();
    GEOPOINT ptLast;

    //地图到显示区坐标转化
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
        ptLast.x = pSrcpt->x;
        ptLast.y = pSrcpt->y;
    }
    int t = (pDespt-pDisplayInfo->GetPointOut());
    if(t>1)
    {
        CGeoRender::Polyline(
            pDisplayInfo->GetCanvas(),
            pDisplayInfo->GetPointOut(), 
            t, style);
    }
}