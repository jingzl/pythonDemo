#ifndef _LABELADJUST_H_
#define _LABELADJUST_H_
#include "LabelAdjustDefine.h"







class CDisplayInfo;
struct stLinkPart;
class IYLGeoNameStyle;
class CCanvas;
class CLabelAdjust
{
public:
	CLabelAdjust();
	~CLabelAdjust();

	BOOL Create( unsigned short iWidth, unsigned short iHeight );
	void Release();

	//��ʼ���Ƴ�ʼ��
	void BeginDraw();

	//��������
	void EndDraw();

	//������ע��
	void DrawLineName( CDisplayInfo* pDisplayInfo, CCanvas* pCanvas, TCHAR* szName, vector< stLinkPart >* pLink, CGeoTextStyle* pTextStyle );

	//����ѡ�е�POI���ע��
	//void DrawSelPoiName(CDisplayInfo* pDisplayInfo,CCanvas* pCanvas, PPoiData SelPoiData ,IYLGeoNameStyle *pStyle);
	//����POI���ע��Ҫ��ﵽ��ѽ��������ڻ���������POI��֮�����
	void DrawPointName( CDisplayInfo* pDisplayInfo, CCanvas* pCanvas, IPointNameListLabel* pIPointNameListLabel, 
        PTILETEXT_INFO pTileTextInfo, int type=DRAW_TEXT );

    // ��������ĵ�
    void DrawSpecialPointName( CDisplayInfo* pDisplayInfo, CCanvas* pCanvas, IPointNameListLabel* pIPointNameListLabel, 
        PTILETEXT_INFO pTileTextInfo, int type=DRAW_TEXT );


protected:
    //���ӽ���
    void AddBan( RECT& r, int nBanJudgeValue );

    //����Ƿ���Ա�ע
    inline unsigned char CanLabel(RECT& r);

    //����Ƿ���Ա�ע
    BOOL IsCanLabel(RECT& r);

    //�õ�һ������Χһ������Ŀ�ע�ǿ�
    BOOL GetLabelRect(POINT& LabelPnt, SIZE& roundSize, SIZE& LabelSize, RECT& OutRect );

    //���ߺ�һ
    void MultiLines2OneLine( vector< stLinkPart >* pLink, stLinkPart* pLinkPart, int* iRetLenght, double Fuzzy );
    void MultiLines2OneLineEx( vector< stLinkPart >* pLink, stLinkPart* pLinkPart, int* iRetLenght, double Fuzzy );

    //��һ��Ϊ���ģ���LABEL_FONT_HEIGHT_SIZEΪ�߳����ɾ���
    inline void CreateRect(POINT& centerPnt, RECT& r ,int& width);
    //��һ��Ϊ���ģ���LABEL_FONT_HEIGHT_SIZEΪ�߳����ɾ���
    inline void CreateRect(POINT& centerPnt, RECT& r ,int width, int height);

    //����ָ���㣬��λ������
    void GetRECT(stLabelPOI& LabelPOI, UINT LabelDirect, RECT& rcDis, BOOL bHasArrow, int nIconWidth, int nIconHeight, int nStrLen );

    //�õ�ע��ƫ��
    int GetTextAlign(int d);

    //����LabelDirect��������������
    void PullOther(stLabelPOI& LabelPOI, UINT LabelDirect,BOOL bDrawArrow, int nIconWidth, int nIconHeight, int nStrLen);

    //ָ������r��ջ
    BOOL Push(RECT& r);

    //�ƶ�����r����
    void Pull(RECT& r);

    //����ͷ
    void RectArrow(HDC hDC, RECT& r, UINT LabelDirect);

    //��ע�����
    //0:RECT ,1:Ellipse
    void DrawFrame(HDC hDC, RECT& r, int type);
    void BackupMatrix(BOOL T);


    // �����ߵ���ĸ����
	void DrawLineAlphabetName( CDisplayInfo* pDisplayInfo, CCanvas* pCanvas, TCHAR* szName, vector< stLinkPart >* pLink, CGeoTextStyle* pTextStyle );

	//�����ߺ�������
	void DrawLineChineseName( CDisplayInfo* pDisplayInfo, CCanvas* pCanvas, TCHAR* szName, vector< stLinkPart >* pLink, CGeoTextStyle* pTextStyle );
	void DrawLineChineseNameEx( CDisplayInfo* pDisplayInfo,	CCanvas* pCanvas, TCHAR* szName, vector< stLinkPart >* pLink, CGeoTextStyle* pTextStyle );


private:
	//��Ƕ����Ƿ���Ч
	BOOL							m_bCreated;

	//ջ����
	unsigned char**					m_iMatrix;	
	//ջ����
	unsigned char**					m_iMatrix2;	
	//ջ�Ŀ�ȣ���Ļ��Сһ��
	short int						m_nWidth;
	//ջ�ĸ߶ȣ���Ļ��Сһ��
	short int						m_nHeight;	
	
	//ע�����ȼ�����
	short int						m_PriorityMatrix[4]; //LABEL_DIRECTS_ENUM_COUNT

	////����ȫ��,for Line
	//int								m_iFontHeight;
	//��ͷ�����ɫ
	HBRUSH							m_hArrowBrush ;
	//��ͷ�߿���ɫ
	HPEN							m_hArrowPen;
	
};

#endif
