#pragma once
#include "DisplayInfo.h"

class CBackRender
{
public:
	CBackRender(void);
	~CBackRender(void);

	void Draw(CDisplayInfo* pDisplayInfo, PMAP_DISP_DATA pData);

    void DrawGridRect(CDisplayInfo* pDisplayInfo, PGRIDRECT_LAYER pGridRectLayer);

protected:

	void DrawLayer(CDisplayInfo* pDisplayInfo,PBACK_LAYER pLayer);
	void DrawAreaFeature(CDisplayInfo* pDisplayInfo, PBACK_FEATURE pFeature, CGeoPolygonStyle& style);
	void DrawLineFeature(CDisplayInfo* pDisplayInfo, PBACK_FEATURE pFeature, CGeoPolylineStyle& style);
    void DrawLineFeature(CDisplayInfo* pDisplayInfo, PGRIDRECT_FEATURE pFeature, CGeoPolylineStyle& style);

private:

    CGeoPolylineStyle m_PolylineStyle;
    CGeoPolygonStyle m_PolygonStyle;

};

