#ifndef _POINTLABELADJUST_H_
#define _POINTLABELADJUST_H_
#include "LabelAdjustDefine.h"



class CDisplayInfo;
class IYLGeoNameStyle;
class CCanvas;
class CPointLabelAdjust
{
public:
	CPointLabelAdjust();
	~CPointLabelAdjust();

    // create / release
	BOOL Create( unsigned short iWidth, unsigned short iHeight );
	void Release();

	// 开始绘制初始化
	void BeginDraw();
	// 结束绘制
	void EndDraw();



    // 仅完成POI图标的回避绘制
	void DrawPoint( CDisplayInfo* pDisplayInfo, CCanvas* pCanvas, IPointNameListLabel* pIPointNameListLabel	);



protected:
    //增加禁区
    void AddBan( RECT& r, int nBanJudgeValue );

    //检测是否可以标注
    inline unsigned char CanLabel(RECT& r);

    //检测是否可以标注
    BOOL IsCanLabel(RECT& r);

    //得到一个点周围一定距离的可注记框
    BOOL GetLabelRect(POINT& LabelPnt, SIZE& roundSize, SIZE& LabelSize, RECT& OutRect );


    //以一点为中心，以LABEL_FONT_HEIGHT_SIZE为边长生成矩形
    inline void CreateRect(POINT& centerPnt, RECT& r, int width, int height);


    //指定区域r进栈
    BOOL Push(RECT& r);

    //制定区域r出棧
    void Pull(RECT& r);



private:
	//标记对象是否有效
	BOOL m_bCreated;

	//栈矩阵
	unsigned char**	m_iMatrix;	

	//栈的宽度和高度，与屏幕大小一致
	short int m_nWidth;
	short int m_nHeight;

	//注记优先级矩阵（四个方向）
	short int m_PriorityMatrix[4];


	
};

#endif
