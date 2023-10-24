#pragma once
#include "DisplayInfo.h"
#include "LabelAdjust.h"


class AnnoLayerPointNameListLabel : public IPointNameListLabel
{
public:
	AnnoLayerPointNameListLabel();
	~AnnoLayerPointNameListLabel();

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
