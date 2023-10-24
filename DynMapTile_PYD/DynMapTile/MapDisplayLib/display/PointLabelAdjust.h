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

	// ��ʼ���Ƴ�ʼ��
	void BeginDraw();
	// ��������
	void EndDraw();



    // �����POIͼ��Ļرܻ���
	void DrawPoint( CDisplayInfo* pDisplayInfo, CCanvas* pCanvas, IPointNameListLabel* pIPointNameListLabel	);



protected:
    //���ӽ���
    void AddBan( RECT& r, int nBanJudgeValue );

    //����Ƿ���Ա�ע
    inline unsigned char CanLabel(RECT& r);

    //����Ƿ���Ա�ע
    BOOL IsCanLabel(RECT& r);

    //�õ�һ������Χһ������Ŀ�ע�ǿ�
    BOOL GetLabelRect(POINT& LabelPnt, SIZE& roundSize, SIZE& LabelSize, RECT& OutRect );


    //��һ��Ϊ���ģ���LABEL_FONT_HEIGHT_SIZEΪ�߳����ɾ���
    inline void CreateRect(POINT& centerPnt, RECT& r, int width, int height);


    //ָ������r��ջ
    BOOL Push(RECT& r);

    //�ƶ�����r����
    void Pull(RECT& r);



private:
	//��Ƕ����Ƿ���Ч
	BOOL m_bCreated;

	//ջ����
	unsigned char**	m_iMatrix;	

	//ջ�Ŀ�Ⱥ͸߶ȣ�����Ļ��Сһ��
	short int m_nWidth;
	short int m_nHeight;

	//ע�����ȼ������ĸ�����
	short int m_PriorityMatrix[4];


	
};

#endif
