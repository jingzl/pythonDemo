#pragma once
#include "DisplayInfo.h"
#include "LabelAdjust.h"


class SpecialLayerPointNameListLabel : public IPointNameListLabel
{
public:
    SpecialLayerPointNameListLabel();
    ~SpecialLayerPointNameListLabel();

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

    void SetLayer(PSPECIAL_LAYER pLyr);

protected:

    PSPECIAL_LAYER pLayer;
    CGeoPointStyle* m_pGeoPointStyle;
    CGeoTextStyle*  m_pGeoTextStyle;

private:

};


class CSpecialPointRender 
{
public:
    CSpecialPointRender();
    ~CSpecialPointRender();


    void Draw(CDisplayInfo* pDisplayInfo, PMAP_DISP_DATA pData, PTILETEXT_INFO pTileTextInfo);

protected:

    void DrawLayer(CDisplayInfo* pDisplayInfo,PSPECIAL_LAYER pLayer, PTILETEXT_INFO pTileTextInfo);


};