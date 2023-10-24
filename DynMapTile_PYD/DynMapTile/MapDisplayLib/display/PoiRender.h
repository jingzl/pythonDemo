#pragma once
#include "DisplayInfo.h"
#include "LabelAdjust.h"
#include "PointLabelAdjust.h"


class POILayerPointNameListLabel : public IPointNameListLabel
{
public:
	POILayerPointNameListLabel();
	~POILayerPointNameListLabel();

	//�ĵ��б��С
	virtual int Size();

	//�õ�һ�������
	virtual GEOPOINT* GetItemPoint(int index);

	//�õ�һ���ע��ֵ
	virtual TCHAR* GetItemText(int index);

	//�õ�����
	virtual CGeoPointStyle* GetPointStyle(int index);

	//�õ�ע�Ƿ��
	virtual CGeoTextStyle* GetTextStyle(int index=0);

	//�Ƿ���ʾ
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
