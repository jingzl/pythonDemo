#include "StdAfx.h"
#include "AnnoRender.h"



AnnoLayerPointNameListLabel::AnnoLayerPointNameListLabel()
{
	pLayer = NULL;
	m_pGeoPointStyle = NULL;
	m_pGeoTextStyle = NULL;
}

AnnoLayerPointNameListLabel::~AnnoLayerPointNameListLabel()
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

//�õ��б��С
int AnnoLayerPointNameListLabel::Size()
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

//�õ�һ�������
GEOPOINT* AnnoLayerPointNameListLabel::GetItemPoint(int index)
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

//�õ�һ���ע��ֵ
TCHAR* AnnoLayerPointNameListLabel::GetItemText(int index)
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

//�õ�����
CGeoPointStyle* AnnoLayerPointNameListLabel::GetPointStyle(int index)
{
	return m_pGeoPointStyle;
}

//�õ�ע�Ƿ��
CGeoTextStyle* AnnoLayerPointNameListLabel::GetTextStyle(int index)
{
	return  m_pGeoTextStyle;
}

void AnnoLayerPointNameListLabel::SetLayer(PANNO_LAYER pLyr)
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

//�Ƿ���ʾ
BOOL AnnoLayerPointNameListLabel::GetDispState(int index)
{
	return TRUE;
}



CAnnoRender::CAnnoRender(void)
{
}

CAnnoRender::~CAnnoRender(void)
{
}

void CAnnoRender::Draw(CDisplayInfo* pDisplayInfo, PMAP_DISP_DATA pData, PTILETEXT_INFO pTileTextInfo)
{
	vector<PANNO_LAYER>::iterator it_anno;
	for ( it_anno=pData->annoLayers.begin(); it_anno!=pData->annoLayers.end(); it_anno++ )
	{
		PANNO_LAYER lpLayer = *it_anno;
		if ( lpLayer != NULL )
		{
			DrawLayer(pDisplayInfo, lpLayer, pTileTextInfo);
		}
	}
}
void CAnnoRender::DrawLayer(CDisplayInfo* pDisplayInfo,PANNO_LAYER pLayer, PTILETEXT_INFO pTileTextInfo)
{
	AnnoLayerPointNameListLabel alpnll;
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
