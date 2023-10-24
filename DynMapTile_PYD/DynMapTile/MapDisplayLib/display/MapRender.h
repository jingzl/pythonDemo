#pragma once
#include "DisplayInfo.h"
#include "RoadRender.h"
#include "BackRender.h"
#include "AnnoRender.h"
#include "PoiRender.h"
#include "SpecialPointRender.h"


class CMapRender
{
public:
	CMapRender(void);
	~CMapRender(void);

	BOOL DrawMap(CDisplayInfo* pDisplayInfo, DWORD dwBackColor, PMAP_DISP_DATA pData, HDC* pDC, 
        PTILETEXT_INFO pTileTextInfo, BOOL bGridLine);


protected:
	void DrawBack(CDisplayInfo* pDisplayInfo, DWORD dwBackColor);


private:

	CRoadRender m_roadRender;
	CBackRender m_backRender;
	CAnnoRender m_annoRender;
	CPoiRender  m_poiRender;
    CSpecialPointRender m_SpecialRender;

};
