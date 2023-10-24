#pragma once
#include "DisplayInfo.h"
#include "LabelAdjust.h"


class AnnoLayerPointNameListLabel : public IPointNameListLabel
{
public:
	AnnoLayerPointNameListLabel();
	~AnnoLayerPointNameListLabel();

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

	void SetLayer(PANNO_LAYER pLyr);

protected:

	PANNO_LAYER pLayer;
	CGeoPointStyle* m_pGeoPointStyle;
	CGeoTextStyle*  m_pGeoTextStyle;


private:


};


class CAnnoRender
{
public:
	CAnnoRender(void);
	~CAnnoRender(void);

	void Draw(CDisplayInfo* pDisplayInfo, PMAP_DISP_DATA pData, PTILETEXT_INFO pTileTextInfo );

private:

	void DrawLayer(CDisplayInfo* pDisplayInfo,PANNO_LAYER pLayer, PTILETEXT_INFO pTileTextInfo);


};
