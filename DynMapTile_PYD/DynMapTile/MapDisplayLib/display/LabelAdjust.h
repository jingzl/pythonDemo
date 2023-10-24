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

	//开始绘制初始化
	void BeginDraw();

	//结束绘制
	void EndDraw();

	//绘制线注记
	void DrawLineName( CDisplayInfo* pDisplayInfo, CCanvas* pCanvas, TCHAR* szName, vector< stLinkPart >* pLink, CGeoTextStyle* pTextStyle );

	//绘制选中的POI点的注记
	//void DrawSelPoiName(CDisplayInfo* pDisplayInfo,CCanvas* pCanvas, PPoiData SelPoiData ,IYLGeoNameStyle *pStyle);
	//绘制POI点标注，要想达到最佳结果，最好在绘制完所有POI点之后调用
	void DrawPointName( CDisplayInfo* pDisplayInfo, CCanvas* pCanvas, IPointNameListLabel* pIPointNameListLabel, 
        PTILETEXT_INFO pTileTextInfo, int type=DRAW_TEXT );

    // 绘制特殊的点
    void DrawSpecialPointName( CDisplayInfo* pDisplayInfo, CCanvas* pCanvas, IPointNameListLabel* pIPointNameListLabel, 
        PTILETEXT_INFO pTileTextInfo, int type=DRAW_TEXT );


protected:
    //增加禁区
    void AddBan( RECT& r, int nBanJudgeValue );

    //检测是否可以标注
    inline unsigned char CanLabel(RECT& r);

    //检测是否可以标注
    BOOL IsCanLabel(RECT& r);

    //得到一个点周围一定距离的可注记框
    BOOL GetLabelRect(POINT& LabelPnt, SIZE& roundSize, SIZE& LabelSize, RECT& OutRect );

    //多线合一
    void MultiLines2OneLine( vector< stLinkPart >* pLink, stLinkPart* pLinkPart, int* iRetLenght, double Fuzzy );
    void MultiLines2OneLineEx( vector< stLinkPart >* pLink, stLinkPart* pLinkPart, int* iRetLenght, double Fuzzy );

    //以一点为中心，以LABEL_FONT_HEIGHT_SIZE为边长生成矩形
    inline void CreateRect(POINT& centerPnt, RECT& r ,int& width);
    //以一点为中心，以LABEL_FONT_HEIGHT_SIZE为边长生成矩形
    inline void CreateRect(POINT& centerPnt, RECT& r ,int width, int height);

    //计算指定点，方位的区域
    void GetRECT(stLabelPOI& LabelPOI, UINT LabelDirect, RECT& rcDis, BOOL bHasArrow, int nIconWidth, int nIconHeight, int nStrLen );

    //得到注记偏移
    int GetTextAlign(int d);

    //将除LabelDirect外的其他区域出棧
    void PullOther(stLabelPOI& LabelPOI, UINT LabelDirect,BOOL bDrawArrow, int nIconWidth, int nIconHeight, int nStrLen);

    //指定区域r进栈
    BOOL Push(RECT& r);

    //制定区域r出棧
    void Pull(RECT& r);

    //画箭头
    void RectArrow(HDC hDC, RECT& r, UINT LabelDirect);

    //画注记外框
    //0:RECT ,1:Ellipse
    void DrawFrame(HDC hDC, RECT& r, int type);
    void BackupMatrix(BOOL T);


    // 绘制线的字母名称
	void DrawLineAlphabetName( CDisplayInfo* pDisplayInfo, CCanvas* pCanvas, TCHAR* szName, vector< stLinkPart >* pLink, CGeoTextStyle* pTextStyle );

	//绘制线汉字名称
	void DrawLineChineseName( CDisplayInfo* pDisplayInfo, CCanvas* pCanvas, TCHAR* szName, vector< stLinkPart >* pLink, CGeoTextStyle* pTextStyle );
	void DrawLineChineseNameEx( CDisplayInfo* pDisplayInfo,	CCanvas* pCanvas, TCHAR* szName, vector< stLinkPart >* pLink, CGeoTextStyle* pTextStyle );


private:
	//标记对象是否有效
	BOOL							m_bCreated;

	//栈矩阵
	unsigned char**					m_iMatrix;	
	//栈矩阵
	unsigned char**					m_iMatrix2;	
	//栈的宽度，屏幕大小一致
	short int						m_nWidth;
	//栈的高度，屏幕大小一致
	short int						m_nHeight;	
	
	//注记优先级矩阵
	short int						m_PriorityMatrix[4]; //LABEL_DIRECTS_ENUM_COUNT

	////字体全高,for Line
	//int								m_iFontHeight;
	//箭头填充颜色
	HBRUSH							m_hArrowBrush ;
	//箭头边框颜色
	HPEN							m_hArrowPen;
	
};

#endif
