#include "stdafx.h"
#include "PointLabelAdjust.h"
#include "DisplayInfo.h"
#include "Canvas.h"
#include "globalfun.h"




//////////////////////////////////////////////////////////////////////////
// 
CPointLabelAdjust::CPointLabelAdjust()
{
	m_bCreated = FALSE;
	m_iMatrix = NULL;	
}

CPointLabelAdjust::~CPointLabelAdjust()
{
	Release();	
}

BOOL CPointLabelAdjust::Create(unsigned short iWidth, unsigned short iHeight)
{
    //产生栈面板
    Release();
    m_iMatrix = new unsigned char*[iHeight];
    for(int i=0; i<iHeight; i++)
    {
        m_iMatrix[i] = new unsigned char[iWidth];
        memset(m_iMatrix[i], 0, sizeof(unsigned char)*iWidth);
    }

    m_nHeight = iHeight;
    m_nWidth = iWidth;

    m_PriorityMatrix[0] = Right;
    m_PriorityMatrix[1] = Left;
    m_PriorityMatrix[2] = Top;
    m_PriorityMatrix[3] = Bottom;

    m_bCreated = TRUE;

    return TRUE;
}

void CPointLabelAdjust::Release()
{
	if(m_iMatrix == NULL) 
	{
		m_bCreated = FALSE;
		return;
	}

	for(int i=0; i<m_nHeight; i++)
	{
		delete [] m_iMatrix[i];
	}
	delete [] m_iMatrix;
	m_iMatrix = NULL;

	m_bCreated = FALSE;
}

void CPointLabelAdjust::BeginDraw()
{
    //开始绘制准备
    if ( !m_bCreated )
    {
        return;
    }

    if ( m_iMatrix == NULL )
    {
        return;
    }
    for ( int i=0; i<m_nHeight; i++ )
    {
        memset(m_iMatrix[i], 0, sizeof(unsigned char)*m_nWidth);
    }
}

void CPointLabelAdjust::EndDraw()
{
    //结束绘制
    return;
}

void CPointLabelAdjust::DrawPoint(CDisplayInfo* pDisplayInfo, CCanvas* pCanvas, IPointNameListLabel* pIPointNameListLabel )
{
    //绘制POI标注
    if ( pIPointNameListLabel->Size() < 1 )
    {
        return;
    }

    //压栈
    vector<stLabelPOI> stLabelPOIList;
    RECT rcDisPic;
    unsigned short		nHeight = 16;	
    unsigned short		nWidth = 16;
    CGeoPointStyle* pPntStyle1 = pIPointNameListLabel->GetPointStyle();
    if(pPntStyle1)
    {
        nHeight = pPntStyle1->m_nHeight;
        nWidth = pPntStyle1->m_nWidth;
    }
    HDC hDC = pCanvas->GetDC();
    CGeoTextStyle* pStyle = pIPointNameListLabel->GetTextStyle();

    BOOL bDrawPoint = TRUE;

    for( int i=0; i<pIPointNameListLabel->Size(); ++i )
    {
        if(pIPointNameListLabel->GetDispState(i) == FALSE)
        {
            continue;
        }
        GEOPOINT* pGeoPnt = pIPointNameListLabel->GetItemPoint(i);

        if( pGeoPnt != NULL )
        {			
            stLabelPOI LabelPOI;
            POINT ptDis = pDisplayInfo->Map2Display(pGeoPnt->x, pGeoPnt->y);
            LabelPOI.m_LabelPnt = ptDis;
            LabelPOI.m_Index = i;

            if(bDrawPoint)
            {
                rcDisPic.left = ptDis.x-nWidth/2;
                rcDisPic.top = ptDis.y - nHeight/2;
                rcDisPic.right = ptDis.x+nWidth/2;
                rcDisPic.bottom = ptDis.y + nHeight/2;
                if(Push(rcDisPic))
                {
                    stLabelPOIList.push_back(LabelPOI);
                }
            }

        }
    }

    //检测注记位置，并注记
    int iLevel = 2;
    BOOL bModify = TRUE;

    while(bModify)
    {//无变化退出
        if(iLevel > 6)
        {
            break;
        }
        bModify = FALSE;
        vector<stLabelPOI>::iterator  it = stLabelPOIList.begin();

        while(it!=stLabelPOIList.end())
        {
            stLabelPOI* pstLabelPOI = &*it;

            rcDisPic.left = pstLabelPOI->m_LabelPnt.x-nWidth/2;
            rcDisPic.top = pstLabelPOI->m_LabelPnt.y - nHeight/2;
            rcDisPic.right = pstLabelPOI->m_LabelPnt.x+nWidth/2;
            rcDisPic.bottom = pstLabelPOI->m_LabelPnt.y + nHeight/2;
            UINT Value1 = CanLabel(rcDisPic);

            if(bDrawPoint)
            {
                if(Value1 < BAN_VALUE)
                {
                    //绘制符号
                    CGeoPointStyle* pPntStyle = pIPointNameListLabel->GetPointStyle(pstLabelPOI->m_Index);
                    CGeoRender::DrawPoint(pCanvas, &pstLabelPOI->m_LabelPnt, pPntStyle);
                    AddBan(rcDisPic, POI_BAN_JUDGE_VALUE);
                }
            }
            it++;
        }
        if(bModify == FALSE)
        {
            bModify = TRUE;
            iLevel++;
        }
    }
}



//////////////////////////////////////////////////////////////////////////
// protected 

void CPointLabelAdjust::AddBan(RECT& r, int nBanJudgeValue)
{
    //加入禁区，永久不被改变
    if(r.left < 0 || r.top < 0 ||  r.right > m_nWidth-1 || r.bottom > m_nHeight-1)
    {
        return ;
    }
    if (r.right + POI_BAN_OUT_WIDTH > nBanJudgeValue || r.bottom + POI_BAN_OUT_HEIGHT > nBanJudgeValue)
    {
        int s = r.right - r.left;
        for(int i=r.top; i<r.bottom; i++)
        {
            memset(&m_iMatrix[i][r.left], POI_MAX_BAN_VALUE, sizeof(unsigned char)*s);
        }
    } 
    else
    {
        int s = r.right - r.left + POI_BAN_OUT_WIDTH;
        for(int i=r.top ; i<r.bottom + POI_BAN_OUT_HEIGHT; i++)
        {
            memset(&m_iMatrix[i][r.left], POI_MAX_BAN_VALUE, sizeof(unsigned char)*s);
        }
    }
}

unsigned char CPointLabelAdjust::CanLabel(RECT& r)
{
    //检测是否可以注记
    if(r.left < 0 || r.top < 0 ||  r.right > m_nWidth-1 || r.bottom > m_nHeight-1)
    {
        return (unsigned char)POI_MAX_BAN_VALUE;
    }

    unsigned char MaxValue = 0;
    for(int j=r.top; j<r.bottom; j++)
    {
        for(int i=r.left; i<r.right; i++)
        {
            MaxValue = m_iMatrix[j][i]>MaxValue ? m_iMatrix[j][i] : MaxValue;
            if(MaxValue > POI_BAN_VALUE)
            {
                return (unsigned char)POI_MAX_BAN_VALUE;
            }
        }
    }
    return MaxValue;
}

BOOL CPointLabelAdjust::IsCanLabel(RECT& r)
{
    //检测是否可以注记
    if(CanLabel(r) > POI_BAN_VALUE)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

void CPointLabelAdjust::CreateRect(POINT& centerPnt, RECT& r ,int width, int height)
{
    r.left = centerPnt.x - width  / 2;
    r.right = r.left + width;
    r.top = centerPnt.y - height / 2;
    r.bottom = r.top + height;
}

BOOL CPointLabelAdjust::Push(RECT& r)
{
    //压栈
    if(r.left < 0 || r.top < 0 ||  r.right > m_nWidth-1 || r.bottom > m_nHeight-1)
    {
        return FALSE;
    }
    for(int i=r.left; i<r.right; i++)
    {
        for(int j=r.top; j<r.bottom; j++)
        {
            m_iMatrix[j][i] += 0x01;
        }
    }
    return TRUE;
}

void CPointLabelAdjust::Pull(RECT& r)
{
    //出栈
    if(r.left < 0 || r.top < 0 ||  r.right > m_nWidth-1 || r.bottom > m_nHeight-1)
    {
        return;
    }

    for(int i=r.left; i<r.right; i++)
    {
        for(int j=r.top; j<r.bottom; j++)
        {
            m_iMatrix[j][i] -= 0x01;
        }
    }
}


