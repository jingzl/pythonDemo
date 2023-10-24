#pragma once
#include "DisplayInfo.h"
#include "LabelAdjust.h"


class CRoadRender
{
public:
	CRoadRender(void);
	~CRoadRender(void);

	void Draw(CDisplayInfo* pDisplayInfo, PMAP_DISP_DATA pData);
	void DrawName(CDisplayInfo* pDisplayInfo, PMAP_DISP_DATA pData);

protected:

	void DrawLayer(CDisplayInfo* pDisplayInfo, PROAD_LAYER pLayer);
	void DrawLayerName(CDisplayInfo* pDisplayInfo, PROAD_LAYER pLayer);


private:

    void DrawFeature(CDisplayInfo* pDisplayInfo, PROAD_LAYER pLayer, PROAD_FEATURE pFeature, CGeoPolylineStyle& style);
	void DrawFeatureName(CDisplayInfo* pDisplayInfo, PROAD_FEATURE pFeature);

	MapRoadLink m_RoadLinks;
	CRITICAL_SECTION m_cs;

    CGeoTextStyle m_TextStyle;
    CGeoPolylineStyle m_PolylineStyle;
    RECT m_rcClip;
    POINT m_minPt;
    POINT m_maxPt;
    int m_LineList[16];

};
