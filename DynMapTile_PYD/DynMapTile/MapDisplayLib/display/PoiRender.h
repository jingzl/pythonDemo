#pragma once
#include "DisplayInfo.h"
#include "LabelAdjust.h"
#include "PointLabelAdjust.h"


class POILayerPointNameListLabel : public IPointNameListLabel
{
public:
	POILayerPointNameListLabel();
	~POILayerPointNameListLabel();

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

	void SetLayer(PPOI_LAYER pLyr);

protected:

	PPOI_LAYER m_pLayer;
	CGeoPointStyle* m_pGeoPointStyle;
	CGeoTextStyle*  m_pGeoTextStyle;

private:

};


class CPoiRender
{
public:
	CPoiRender(void);
	~CPoiRender(void);

	void Draw(CDisplayInfo* pDisplayInfo, PMAP_DISP_DATA pData, PTILETEXT_INFO pTileTextInfo);


protected:
    void DrawLayer(CDisplayInfo* pDisplayInfo, PPOI_LAYER lpLayer, PTILETEXT_INFO pTileTextInfo);

};
