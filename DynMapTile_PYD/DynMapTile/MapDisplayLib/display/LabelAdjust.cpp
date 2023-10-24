#include "stdafx.h"
#include "LabelAdjust.h"
#include "DisplayInfo.h"
#include "Canvas.h"
#include "globalfun.h"




//////////////////////////////////////////////////////////////////////////
// 
CLabelAdjust::CLabelAdjust()
{
	m_bCreated = FALSE;
	m_iMatrix = NULL;	
}

CLabelAdjust::~CLabelAdjust()
{
	Release();	
}

BOOL CLabelAdjust::Create(unsigned short iWidth, unsigned short iHeight)
{
    //产生栈面板
    Release();
    m_iMatrix = new unsigned char*[iHeight];
    m_iMatrix2 = new unsigned char*[iHeight];

    for(int i=0; i<iHeight; i++)
    {
        m_iMatrix[i] = new unsigned char[iWidth];
        memset(m_iMatrix[i], 0, sizeof(unsigned char)*iWidth);
    }
    for(int i=0; i<iHeight; i++)
    {
        m_iMatrix2[i] = new unsigned char[iWidth];
        memset(m_iMatrix2[i], 0, sizeof(unsigned char)*iWidth);
    }

    m_nHeight = iHeight;
    m_nWidth = iWidth;
    m_bCreated = TRUE;

    m_PriorityMatrix[0] = Right;
    m_PriorityMatrix[1] = Left;
    m_PriorityMatrix[2] = Top;
    m_PriorityMatrix[3] = Bottom;

    return TRUE;
}

void CLabelAdjust::Release()
{
	if(m_iMatrix == NULL) 
	{
		m_bCreated = FALSE;
		return;
	}

	for(int i=0; i<m_nHeight; i++)
	{
		delete []m_iMatrix[i];
	}
	delete []m_iMatrix;
	m_iMatrix = NULL;

	for(int i=0; i<m_nHeight; i++)
	{
		delete []m_iMatrix2[i];
	}
	delete []m_iMatrix2;
	m_iMatrix2 = NULL;

	m_bCreated = FALSE;
}

void CLabelAdjust::BeginDraw()
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

void CLabelAdjust::EndDraw()
{
    //结束绘制
    return;
}

void CLabelAdjust::DrawLineName( CDisplayInfo* pDisplayInfo, CCanvas* pCanvas, TCHAR* strName, vector<stLinkPart>* pLink, CGeoTextStyle* pTextStyle )
{
    if ( IsAlphabet(strName) )
    {
        TCHAR szName[64] = {0};
        ToAlphabet( strName, szName );
        DrawLineAlphabetName(pDisplayInfo, pCanvas,szName,  pLink, pTextStyle);
    }
    else
    {
        // 优化
        DrawLineChineseNameEx(pDisplayInfo, pCanvas,strName, pLink, pTextStyle);
        // 下面的方法会出现重字及道路名由下到上反向走的情况
        //DrawLineChineseName(pDisplayInfo, pCanvas,strName, pLink, pTextStyle);
    }
}

void CLabelAdjust::DrawPointName(CDisplayInfo* pDisplayInfo, CCanvas* pCanvas, IPointNameListLabel* pIPointNameListLabel, 
                                 PTILETEXT_INFO pTileTextInfo, int type )
{
    //绘制POI标注
    if ( pIPointNameListLabel->Size() < 1 )
    {
        return;
    }

    //压栈
    vector<stLabelPOI> stLabelPOIList;
    RECT rcDis;
    RECT rcDisPic;
    unsigned short		nHeight = 16;	
    unsigned short		nWidth = 16;
    CGeoPointStyle* pPntStyle = pIPointNameListLabel->GetPointStyle(0);
    if ( pPntStyle == NULL )
    {
        return;
    }
    CGeoTextStyle* pGeoTextStyle = pIPointNameListLabel->GetTextStyle();
    if ( pGeoTextStyle == NULL )
    {
        return;
    }
    nHeight = pPntStyle->m_nHeight;
    nWidth = pPntStyle->m_nWidth;

    HDC hDC = pCanvas->GetDC();
    m_hArrowBrush = ::CreateSolidBrush(pGeoTextStyle->m_clrBack);
    m_hArrowPen = ::CreatePen(PS_SOLID, pGeoTextStyle->m_nEdgeWidth, pGeoTextStyle->m_clrRoundLine);
 
    BOOL bDrawPoint = (type&DRAW_POINT_SYMBOL)>0 ? TRUE : FALSE;
    BOOL bDrawText = (type&DRAW_TEXT)>0 ? TRUE : FALSE;
    BOOL bDrawArrow = (pGeoTextStyle->m_drawtype&eTEXT_DT_DRAWARROWDIR)>0 ? TRUE : FALSE;

    for( int i=0; i<pIPointNameListLabel->Size(); ++i )
    {
        if(pIPointNameListLabel->GetDispState(i) == FALSE)
        {
            continue;
        }
        GEOPOINT* pGeoPnt = pIPointNameListLabel->GetItemPoint(i);
        if( pGeoPnt != NULL )
        {
            POINT ptDis = pDisplayInfo->Map2Display(pGeoPnt->x, pGeoPnt->y);
            TCHAR * name = pIPointNameListLabel->GetItemText(i);
            stLabelPOI LabelPOI;
            LabelPOI.m_LabelPnt = ptDis;
            LabelPOI.m_Index = i;
            BOOL bTxt = FALSE; // 字符串的长度是否为空
            if ( name != NULL )
            {
                GetTextExtentPoint32(hDC, name, _tcslen(name), &LabelPOI.m_Size);
                bTxt = TRUE;
            }
            else
            {
                bTxt = FALSE;
            }
            if(bDrawText && bTxt)
            {
                for(int j=0; j<LABEL_DIRECTS_ENUM_COUNT; j++)
                {
                    GetRECT(LabelPOI, m_PriorityMatrix[j], rcDis, bDrawArrow, nWidth, nHeight, _tcslen(name) );
                    if(Push(rcDis))
                    {
                        LabelPOI.m_LabelDirect |= m_PriorityMatrix[j];
                    }
                }
                if(LabelPOI.m_LabelDirect > 0)
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
            else
            {
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
    }

    //检测注记位置，并注记
    int iLevel = 2;
    BOOL bModify = TRUE;

    while ( bModify && stLabelPOIList.size() > 0 )
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
            BOOL bErase = FALSE;

            GEOPOINT* pGeoPnt = pIPointNameListLabel->GetItemPoint(pstLabelPOI->m_Index);
            TCHAR * name = pIPointNameListLabel->GetItemText(pstLabelPOI->m_Index);

            rcDisPic.left = pstLabelPOI->m_LabelPnt.x-nWidth/2;
            rcDisPic.top = pstLabelPOI->m_LabelPnt.y - nHeight/2;
            rcDisPic.right = pstLabelPOI->m_LabelPnt.x+nWidth/2;
            rcDisPic.bottom = pstLabelPOI->m_LabelPnt.y + nHeight/2;
            UINT Value1 = CanLabel(rcDisPic);

            if( bDrawText && name != NULL )
            {
                if(Value1 < BAN_VALUE)
                {
                    for(int i=0; i<LABEL_DIRECTS_ENUM_COUNT; i++)
                    {
                        if((pstLabelPOI->m_LabelDirect & m_PriorityMatrix[i]) == m_PriorityMatrix[i])
                        {
                            GetRECT(*pstLabelPOI, m_PriorityMatrix[i], rcDis,bDrawArrow, nWidth, nHeight, _tcslen(name));
                            UINT Value = CanLabel(rcDis);

                            if(Value < BAN_VALUE)
                            {
                                if( pGeoPnt != NULL )
                                {
                                    if(bDrawPoint)
                                    {
                                        //绘制符号
                                        CGeoRender::DrawPoint(pCanvas, &pstLabelPOI->m_LabelPnt, pPntStyle);
                                    }
                                    if((pGeoTextStyle->m_drawtype&eTEXT_DT_DRAWARROWDIR) > 0)
                                    {
                                        RectArrow(hDC, rcDis, m_PriorityMatrix[i]);
                                    }

                                    // 采用多行标注，则必须使用 DT_WORDBREAK 参数
                                    if((pGeoTextStyle->m_drawtype&eTEXT_DT_OUTLINEFONT) > 0 )
                                    {
                                        pCanvas->DrawEdgeText(name, wcslen(name), &rcDis, pGeoTextStyle->m_clrFont, pGeoTextStyle->m_clrBack, GetTextAlign(m_PriorityMatrix[i]));
                                    }
                                    else
                                    {
                                        ::DrawText(hDC, name, -1, &rcDis, GetTextAlign(m_PriorityMatrix[i]));
                                    }

                                    if((pGeoTextStyle->m_drawtype&eTEXT_DT_STRINGROUND) > 0 )
                                    {
                                        if((pGeoTextStyle->m_drawtype&eTEXT_DT_ROUNDTYPE) > 0)
                                        {
                                            //绘制椭圆
                                            DrawFrame(hDC, rcDis, 1);
                                        }
                                        else
                                        {
                                            //绘制矩形
                                            DrawFrame(hDC, rcDis, 0);
                                        }
                                    }

                                    //解锁别的位置
                                    PullOther(*pstLabelPOI, m_PriorityMatrix[i], bDrawArrow, nWidth, nHeight, _tcslen(name));
                                    //设为禁区
                                    AddBan(rcDis, BAN_PNTNAME_JUDGE_VALUE);
                                    AddBan(rcDisPic, BAN_PNTNAME_JUDGE_VALUE);

                                    // 收集
                                    if ( pTileTextInfo != NULL )
                                    {
                                        PTILETEXT_ITEM  pTextItem = new ST_TILETEXT_ITEM();
                                        if ( pTextItem != NULL )
                                        {
                                            _stprintf( pTextItem->m_szID, _T("0") );
                                            pTextItem->m_pText = new TCHAR [wcslen(name)+1];
                                            memset( pTextItem->m_pText, 0, sizeof(TCHAR)*(wcslen(name)+1) );
                                            wcscpy( pTextItem->m_pText, name );
                                            pTextItem->m_nTxtVisable = 1;
                                            _stprintf( pTextItem->m_szKind, _T("%04X"), pPntStyle->m_nSubCode );
                                            pTextItem->m_ptShape = *pGeoPnt;

                                            RECT rc;
                                            UnionRect( &rc, &rcDis, &rcDisPic );

                                            GEOPOINT geoPt;
                                            POINT pt;
                                            pt.x = rc.left;
                                            pt.y = rc.bottom;
                                            pDisplayInfo->Src2Map( pt, geoPt );
                                            pTextItem->m_rc.dMinX = geoPt.x;
                                            pTextItem->m_rc.dMinY = geoPt.y;
                                            pt.x = rc.right;
                                            pt.y = rc.top;
                                            pDisplayInfo->Src2Map( pt, geoPt );
                                            pTextItem->m_rc.dMaxX = geoPt.x;
                                            pTextItem->m_rc.dMaxY = geoPt.y;

                                            pTextItem->m_ico.left = rcDisPic.left - rc.left;
                                            pTextItem->m_ico.top = rc.bottom - rcDisPic.bottom;
                                            pTextItem->m_ico.right = rcDisPic.right - rcDisPic.left;
                                            pTextItem->m_ico.bottom = rcDisPic.bottom - rcDisPic.top;

                                            pTextItem->m_txt.left = rcDis.left - rc.left;
                                            pTextItem->m_txt.top = rc.bottom - rcDis.bottom;
                                            pTextItem->m_txt.right = rcDis.right - rcDis.left;
                                            pTextItem->m_txt.bottom = rcDis.bottom - rcDis.top;

                                            pTileTextInfo->m_TileTextItemAry.push_back( pTextItem );
                                        }
                                    }
                                }
                                bErase = TRUE;
                                bModify = TRUE;
                                break;
                            }
                            else 
                            {
                                //排除重叠地图要素点区域，碰到禁区
                                Pull(rcDis);
                                pstLabelPOI->m_LabelDirect -= m_PriorityMatrix[i];
                                bModify = TRUE;
                            }
                        }
                    }
                }
                else
                {
                    bErase = TRUE;
                    bModify = TRUE;
                }
            }
            else
            {
                if(bDrawPoint)
                {
                    if(Value1 < BAN_VALUE)
                    {
                        //绘制符号
                        CGeoRender::DrawPoint(pCanvas, &pstLabelPOI->m_LabelPnt, pPntStyle);
                        AddBan(rcDisPic, BAN_PNTNAME_JUDGE_VALUE);

                        // 收集
                        if ( pTileTextInfo != NULL )
                        {
                            PTILETEXT_ITEM  pTextItem = new ST_TILETEXT_ITEM();
                            if ( pTextItem != NULL )
                            {
                                _stprintf( pTextItem->m_szID, _T("0") );
                                pTextItem->m_pText = NULL;
                                pTextItem->m_nTxtVisable = 0;
                                _stprintf( pTextItem->m_szKind, _T("%04X"), pPntStyle->m_nSubCode );
                                pTextItem->m_ptShape = *pGeoPnt;

                                RECT rc;
                                UnionRect( &rc, &rcDisPic, &rcDisPic );

                                GEOPOINT geoPt;
                                POINT pt;
                                pt.x = rc.left;
                                pt.y = rc.bottom;
                                pDisplayInfo->Src2Map( pt, geoPt );
                                pTextItem->m_rc.dMinX = geoPt.x;
                                pTextItem->m_rc.dMinY = geoPt.y;
                                pt.x = rc.right;
                                pt.y = rc.top;
                                pDisplayInfo->Src2Map( pt, geoPt );
                                pTextItem->m_rc.dMaxX = geoPt.x;
                                pTextItem->m_rc.dMaxY = geoPt.y;

                                pTextItem->m_ico.left = rcDisPic.left - rc.left;
                                pTextItem->m_ico.top = rc.bottom - rcDisPic.bottom;
                                pTextItem->m_ico.right = rcDisPic.right - rcDisPic.left;
                                pTextItem->m_ico.bottom = rcDisPic.bottom - rcDisPic.top;

                                pTileTextInfo->m_TileTextItemAry.push_back( pTextItem );
                            }
                        }
                    }
                }
            }

            if(bErase == FALSE)
            {
                it++;
            }
            else
            {
                it = stLabelPOIList.erase(it);
            }
        }
        if(bModify == FALSE)
        {
            bModify = TRUE;
            iLevel++;
        }
    }
    ::DeleteObject(m_hArrowBrush);
    ::DeleteObject(m_hArrowPen);
}

void CLabelAdjust::DrawSpecialPointName( CDisplayInfo* pDisplayInfo, CCanvas* pCanvas, IPointNameListLabel* pIPointNameListLabel, 
                                        PTILETEXT_INFO pTileTextInfo, int type )
{
    // 目前没有使用
    return;

    // 目前处理和DrawPointName一样
    if ( pIPointNameListLabel->Size() < 1 )
    {
        return;
    }

    //压栈
    vector<stLabelPOI> stLabelPOIList;
    RECT rcDis;
    RECT rcDisPic;
    unsigned short		nHeight = 16;	
    unsigned short		nWidth = 16;
    CGeoPointStyle* pPntStyle1 = pIPointNameListLabel->GetPointStyle(0);
    if(pPntStyle1)
    {
        nHeight = pPntStyle1->m_nHeight;
        nWidth = pPntStyle1->m_nWidth;
    }
    HDC hDC = pCanvas->GetDC();
    CGeoTextStyle* pStyle = pIPointNameListLabel->GetTextStyle();
    HPEN  hSelPen;
    hSelPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    m_hArrowBrush = ::CreateSolidBrush(pStyle->m_clrBack);
    m_hArrowPen = ::CreatePen(PS_SOLID, pStyle->m_nEdgeWidth, pStyle->m_clrRoundLine);

    BOOL bDrawPoint = (type&DRAW_POINT_SYMBOL)>0 ? TRUE : FALSE;
    BOOL bDrawText = (type&DRAW_TEXT)>0 ? TRUE : FALSE;
    BOOL bDrawArrow = (pStyle->m_drawtype&eTEXT_DT_DRAWARROWDIR)>0 ? TRUE : FALSE;

    for ( int i=0; i<pIPointNameListLabel->Size(); ++i )
    {
        if ( pIPointNameListLabel->GetDispState(i) == FALSE )
        {
            continue;
        }

        GEOPOINT* pGeoPnt = pIPointNameListLabel->GetItemPoint(i);
        TCHAR * name = pIPointNameListLabel->GetItemText(i);
        if( pGeoPnt != NULL )
        {			
            stLabelPOI LabelPOI;
            POINT ptDis = pDisplayInfo->Map2Display(pGeoPnt->x, pGeoPnt->y);
            LabelPOI.m_LabelPnt = ptDis;
            LabelPOI.m_Index = i;
            BOOL bTxt = FALSE; // 字符串的长度是否为空
            if ( name != NULL )
            {
                GetTextExtentPoint32(hDC, name, _tcslen(name), &LabelPOI.m_Size);
                bTxt = TRUE;
            }
            else
            {
                bTxt = FALSE;
            }
            if(bDrawText && bTxt)
            {
                for(int j=0; j<LABEL_DIRECTS_ENUM_COUNT; j++)
                {
                    GetRECT(LabelPOI, m_PriorityMatrix[j], rcDis, bDrawArrow, nWidth, nHeight, _tcslen(name));
                    if(Push(rcDis))
                    {
                        LabelPOI.m_LabelDirect |= m_PriorityMatrix[j];
                    }
                }
                if(LabelPOI.m_LabelDirect > 0)
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
            else
            {
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
            BOOL bErase = FALSE;
            GEOPOINT* pGeoPnt = pIPointNameListLabel->GetItemPoint(pstLabelPOI->m_Index);
            TCHAR * name = pIPointNameListLabel->GetItemText(pstLabelPOI->m_Index);

            rcDisPic.left = pstLabelPOI->m_LabelPnt.x-nWidth/2;
            rcDisPic.top = pstLabelPOI->m_LabelPnt.y - nHeight/2;
            rcDisPic.right = pstLabelPOI->m_LabelPnt.x+nWidth/2;
            rcDisPic.bottom = pstLabelPOI->m_LabelPnt.y + nHeight/2;
            UINT Value1 = CanLabel(rcDisPic);

            if ( bDrawText && name != NULL )
            {
                if(Value1 < BAN_VALUE)
                {
                    for(int i=0; i<LABEL_DIRECTS_ENUM_COUNT; i++)
                    {
                        if((pstLabelPOI->m_LabelDirect & m_PriorityMatrix[i]) == m_PriorityMatrix[i])
                        {
                            CGeoTextStyle* pCGeoTextStyle = pIPointNameListLabel->GetTextStyle(pstLabelPOI->m_Index);
                            //pCGeoTextStyle->InitStyle(pCanvas);

                            GetRECT(*pstLabelPOI, m_PriorityMatrix[i], rcDis,bDrawArrow , nWidth, nHeight, _tcslen(name));
                            UINT Value = CanLabel(rcDis);

                            if(Value < BAN_VALUE)
                            {
                                if( pGeoPnt != NULL )
                                {
                                    if(bDrawPoint)
                                    {
                                        //绘制符号
                                        CGeoPointStyle* pPntStyle = pIPointNameListLabel->GetPointStyle(pstLabelPOI->m_Index);
                                        //pPntStyle->InitStyle(pCanvas);
                                        CGeoRender::DrawPoint(pCanvas, &pstLabelPOI->m_LabelPnt, pPntStyle);
                                    }
                                    if((pCGeoTextStyle->m_drawtype&eTEXT_DT_DRAWARROWDIR) > 0)
                                    {
                                        RectArrow(hDC, rcDis, m_PriorityMatrix[i]);
                                    }

                                    // 采用多行标注，则必须使用 DT_WORDBREAK 参数
                                    if((pCGeoTextStyle->m_drawtype&eTEXT_DT_OUTLINEFONT) > 0 )
                                    {
                                        pCanvas->DrawEdgeText(name, wcslen(name), &rcDis, pCGeoTextStyle->m_clrFont, pCGeoTextStyle->m_clrBack, GetTextAlign(m_PriorityMatrix[i]));
                                    }
                                    else
                                    {
                                        ::DrawText(hDC, name, -1, &rcDis, GetTextAlign(m_PriorityMatrix[i]));
                                    }

                                    if((pCGeoTextStyle->m_drawtype&eTEXT_DT_STRINGROUND) > 0 )
                                    {
                                        if((pCGeoTextStyle->m_drawtype&eTEXT_DT_ROUNDTYPE) > 0)
                                        {
                                            //绘制椭圆
                                            DrawFrame(hDC, rcDis, 1);
                                        }
                                        else
                                        {
                                            //绘制矩形
                                            DrawFrame(hDC, rcDis, 0);
                                        }

                                    }

                                    //解锁别的位置
                                    PullOther(*pstLabelPOI, m_PriorityMatrix[i], bDrawArrow, nWidth, nHeight, _tcslen(name));
                                    //设为禁区
                                    AddBan(rcDis, BAN_LINENAME_JUDGE_VALUE);
                                    AddBan(rcDisPic, BAN_LINENAME_JUDGE_VALUE);
                                }
                                bErase = TRUE;
                                bModify = TRUE;
                                break;
                            }
                            else 
                            {
                                //排除重叠地图要素点区域，碰到禁区
                                Pull(rcDis);
                                pstLabelPOI->m_LabelDirect -= m_PriorityMatrix[i];
                                bModify = TRUE;
                            }
                            //pCGeoTextStyle->UnInitStyle(pCanvas);
                        }
                    }
                }
                else
                {
                    bErase = TRUE;
                    bModify = TRUE;
                }
            }
            else
            {
                if(bDrawPoint)
                {
                    if(Value1 < BAN_VALUE)
                    {
                        //绘制符号
                        CGeoPointStyle* pPntStyle = pIPointNameListLabel->GetPointStyle(pstLabelPOI->m_Index);
                        //pPntStyle->InitStyle(pCanvas);
                        CGeoRender::DrawPoint(pCanvas, &pstLabelPOI->m_LabelPnt, pPntStyle);
                        AddBan(rcDisPic, BAN_LINENAME_JUDGE_VALUE);
                    }
                }
            }

            if(bErase == FALSE)
            {
                it++;
            }
            else
            {
                it = stLabelPOIList.erase(it);
            }
        }
        if(bModify == FALSE)
        {
            bModify = TRUE;
            iLevel++;
        }
    }

    ::DeleteObject(m_hArrowBrush);
    ::DeleteObject(m_hArrowPen);
    ::DeleteObject(hSelPen);
}








//////////////////////////////////////////////////////////////////////////
// protected 

void CLabelAdjust::AddBan(RECT& r, int nBanJudgeValue)
{
    //加入禁区，永久不被改变
    if(r.left < 0 || r.top < 0 ||  r.right > m_nWidth-1 || r.bottom > m_nHeight-1)
    {
        return ;
    }
    if (r.right + BAN_OUT_WIDTH > nBanJudgeValue || r.bottom + BAN_OUT_HEIGHT > nBanJudgeValue)
    {
        int s = r.right - r.left;
        for(int i=r.top; i<r.bottom; i++)
        {
            memset(&m_iMatrix[i][r.left], MAX_BAN_VALUE, sizeof(unsigned char)*s);
        }
    } 
    else
    {
        // 越界判断
        int s = r.right - r.left + BAN_OUT_WIDTH;
        if ( s + r.left > m_nWidth - 1 )
        {
            s = r.right - r.left;
        }
        int s2 = r.bottom + BAN_OUT_HEIGHT;
        if ( s2 > m_nHeight - 1 )
        {
            s2 = m_nHeight - 1;
        }
        for(int i=r.top ; i<s2; i++)
        {
            memset( &m_iMatrix[i][r.left], MAX_BAN_VALUE, sizeof(unsigned char)*s );
        }
    }
}

unsigned char CLabelAdjust::CanLabel(RECT& r)
{
    //检测是否可以注记
    if(r.left < 0 || r.top < 0 ||  r.right > m_nWidth-1 || r.bottom > m_nHeight-1)
    {
        return (unsigned char)MAX_BAN_VALUE;
    }

    unsigned char MaxValue = 0;
    for(int j=r.top; j<r.bottom; j++)
    {
        for(int i=r.left; i<r.right; i++)
        {
            MaxValue = m_iMatrix[j][i]>MaxValue ? m_iMatrix[j][i] : MaxValue;
            if(MaxValue > BAN_VALUE)
            {
                return (unsigned char)MAX_BAN_VALUE;
            }
        }
    }
    return MaxValue;
}

BOOL CLabelAdjust::IsCanLabel(RECT& r)
{
    //检测是否可以注记
    if(CanLabel(r) > BAN_VALUE)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

BOOL CLabelAdjust::GetLabelRect(POINT& LabelPnt, SIZE& roundSize, SIZE& LabelSize, RECT& OutRect )
{
    BOOL bOK = FALSE;
    RECT rcDis;
    for(int i=0; i<LABEL_DIRECTS_ENUM_COUNT; i++)
    {
        switch(m_PriorityMatrix[i])
        {
        case Right:
            {
                rcDis.left = LabelPnt.x + roundSize.cx / 2;
                rcDis.top = LabelPnt.y - LabelSize.cy / 2;
                rcDis.right = rcDis.left + LabelSize.cx;
                rcDis.bottom =  rcDis.top + LabelSize.cy;
                break;
            }

        case Top:
            {
                rcDis.left = LabelPnt.x - LabelSize.cx / 2;
                rcDis.top = LabelPnt.y - roundSize.cy / 2 - LabelSize.cy;
                rcDis.right = rcDis.left + LabelSize.cx;
                rcDis.bottom =  rcDis.top + LabelSize.cy;
                break;
            }

        case Left:
            {
                rcDis.left = LabelPnt.x - roundSize.cx / 2 - LabelSize.cx;
                rcDis.top = LabelPnt.y - LabelSize.cy / 2;
                rcDis.right = rcDis.left + LabelSize.cx;
                rcDis.bottom =  rcDis.top + LabelSize.cy;
                break;
            }

        case Bottom:
            {
                rcDis.left = LabelPnt.x - LabelSize.cx / 2;
                rcDis.top = LabelPnt.y + roundSize.cy / 2;
                rcDis.right = rcDis.left + LabelSize.cx;
                rcDis.bottom =  rcDis.top + LabelSize.cy;
                break;
            }
        }

        if(IsCanLabel(rcDis))
        {
            OutRect = rcDis;
            bOK = TRUE;
            break;
        }
    }
    return bOK;
}

void CLabelAdjust::MultiLines2OneLine(vector< stLinkPart >* pLink, stLinkPart* pLinkPart, int* iRetLenght, double Fuzzy)
{	
    // 	TRACE(L"\nStart MultiLines2OneLine  %d \n", pLink->size());
    vector<int> indexList;
    vector<MergeNodeInfo> MergeLineInfoList;
    stLinkPart pstLinkPart;
    stLinkPart pstLinkParts;
    stLinkPart pstLinkParte;
    vector<int>::iterator  it;
    int i = 0;

    if(pLink->size() == 0)
    {
        return;
    }
    int dis =0;
    int index= -1;
    for (i=0; i<pLink->size(); i++)
    {
        pstLinkPart = pLink->at(i);
        if(pstLinkPart.m_iTotal > dis)
        {
            dis = pstLinkPart.m_iTotal;
            index = i;
        }
    }


    //建立索引表
    for ( i=0; i<pLink->size(); i++)
    {
        indexList.push_back(i);
    }

    //加入种子
    if(index > -1)
    {
        pstLinkPart = pLink->at(index);
    }
    else
    {
        index = 0;
        pstLinkPart = pLink->at(0);
    }
    int nodecount = 0;
    MergeNodeInfo mni1;
    MergeNodeInfo mni2;
    mni1.iNextLineIndex = index;
    mni1.iNextLinePntIndex = 0;
    mni2.iPreLineIndex = index;
    mni2.iPreLinePntIndex = pstLinkPart.m_iPointNum-1;
    nodecount += pstLinkPart.m_iPointNum;
    MergeLineInfoList.push_back(mni1);
    MergeLineInfoList.push_back(mni2);

    //生成合并信息列表
    int lowpos,uppos,iSize;
    POINT p1,p2;
    POINT q1,q2;
    BOOL bModify = TRUE;
    iSize = 2;

    while(bModify)
    {
        bModify = FALSE;
        it = indexList.begin();

        while(it!=indexList.end())
        {
            lowpos = 0;
            uppos = iSize-1;
            i = *it;
            pstLinkPart = pLink->at(i);
            if(pstLinkPart.m_iPointNum < 2 || i == MergeLineInfoList[uppos].iPreLineIndex || i == MergeLineInfoList[lowpos].iNextLineIndex)
            {
                it++;
                continue;
            }
            //取起点
            pstLinkParts = pLink->at(MergeLineInfoList[lowpos].iNextLineIndex);
            q1 = pstLinkParts.m_pLinkPtr[MergeLineInfoList[lowpos].iNextLinePntIndex];

            //取终点
            pstLinkParte = pLink->at(MergeLineInfoList[uppos].iPreLineIndex);
            q2 = pstLinkParte.m_pLinkPtr[MergeLineInfoList[uppos].iPreLinePntIndex];

            //取一条线
            p1 = pstLinkPart.m_pLinkPtr[0];
            p2 = pstLinkPart.m_pLinkPtr[pstLinkPart.m_iPointNum-1];
            BOOL bCan1,bCan2,bCan3,bCan4;
            //TRACE("\n*** q1(%d,%d) q2(%d,%d), a Line:p1(%d,%d), p2(%d,%d)\n",q1.x, q1.y, q2.x,q2.y, p1.x,p1.y, p2.x, p2.y);
            bCan1 = CanMerge(&q1, &p1, &Fuzzy);
            bCan2 = CanMerge(&q1, &p2, &Fuzzy);
            bCan3 = CanMerge(&q2, &p1, &Fuzzy);
            bCan4 = CanMerge(&q2, &p2, &Fuzzy);
            int re = bCan1 + bCan2 + bCan3 + bCan4;
            if(re > 1)
            {
                it++;
                continue;
            }
            if(bCan1)
            {
                MergeLineInfoList[lowpos].iPreLineIndex = i;
                MergeLineInfoList[lowpos].iPreLinePntIndex = 0;
                mni1.iNextLineIndex = i;
                mni1.iNextLinePntIndex = pstLinkPart.m_iPointNum -1;
                MergeLineInfoList.insert(MergeLineInfoList.begin(),mni1);
                bModify = TRUE;
                nodecount += pstLinkPart.m_iPointNum;
                iSize++;
                it = indexList.erase(it);
                continue;
            }
            if(bCan2)
            {
                MergeLineInfoList[lowpos].iPreLineIndex = i;
                MergeLineInfoList[lowpos].iPreLinePntIndex = pstLinkPart.m_iPointNum -1;
                mni1.iNextLineIndex = i;
                mni1.iNextLinePntIndex = 0;
                MergeLineInfoList.insert(MergeLineInfoList.begin(),mni1);
                bModify = TRUE;
                nodecount += pstLinkPart.m_iPointNum;
                iSize++;
                it = indexList.erase(it);
                continue;
            }
            if(bCan3)
            {
                MergeLineInfoList[uppos].iNextLineIndex = i;
                MergeLineInfoList[uppos].iNextLinePntIndex = 0;
                mni2.iPreLineIndex = i;
                mni2.iPreLinePntIndex = pstLinkPart.m_iPointNum-1;
                MergeLineInfoList.push_back(mni2);
                bModify = TRUE;
                nodecount += pstLinkPart.m_iPointNum;
                iSize++;
                it = indexList.erase(it);
                continue;
            }
            if(bCan4)
            {
                MergeLineInfoList[uppos].iNextLineIndex = i;
                MergeLineInfoList[uppos].iNextLinePntIndex = pstLinkPart.m_iPointNum-1;
                mni2.iPreLineIndex = i;
                mni2.iPreLinePntIndex = 0;
                MergeLineInfoList.push_back(mni2);
                bModify = TRUE;
                nodecount += pstLinkPart.m_iPointNum;
                iSize++;
                it = indexList.erase(it);
                continue;
            }
            it++;
        }
    }

    indexList.clear();
    //TRACE(L"\nStart Copy line\n");

    //拷贝到一个列表里
    pLinkPart->m_pLinkPtr = new POINT[nodecount];

    pLinkPart->m_iPointNum = nodecount;
    short LineIndex,startPos,endPos,nodeIndex;
    LineIndex = MergeLineInfoList[0].iNextLineIndex;
    startPos = MergeLineInfoList[0].iNextLinePntIndex;
    nodeIndex = 0;

    for(i=1; i<MergeLineInfoList.size(); i++)
    {
        endPos = MergeLineInfoList[i].iPreLinePntIndex;
        pstLinkPart =  pLink->at(LineIndex);
        if(startPos < endPos)
        {
            for (int j=startPos; j<=endPos; j++)
            {
                pLinkPart->m_pLinkPtr[nodeIndex] =pstLinkPart.m_pLinkPtr[j];
                nodeIndex++;
            }
        }
        else
        {
            for (int j=endPos; j>=startPos; j--)
            {
                pLinkPart->m_pLinkPtr[nodeIndex] =pstLinkPart.m_pLinkPtr[j];
                nodeIndex++;
            }
        }
        LineIndex = MergeLineInfoList[i].iNextLineIndex;
        startPos = MergeLineInfoList[i].iNextLinePntIndex;
    }
    *iRetLenght = nodeIndex;
    MergeLineInfoList.clear();
    // 	TRACE(L"\nEnd Copy line\n");
}

void CLabelAdjust::MultiLines2OneLineEx(vector< stLinkPart >* pLink, stLinkPart* pLinkPart, int* iRetLenght, double Fuzzy)
{	
    //构造最长路径数据结构
    *iRetLenght = 0;

    vector<POINT> NodeList;
    vector<LineScript>LineList;

    //TRACE("\n pLink->size = %d \n", pLink->size());
    for (int ii=0; ii<pLink->size(); ii++)
    {
        stLinkPart plink = pLink->at(ii);
        POINT p1 = plink.m_pLinkPtr[0];
        POINT p2 = plink.m_pLinkPtr[plink.m_iPointNum-1];
        if(p1.x == p2.x && p1.y == p2.y)
        {
            continue;
        }

        int jk,index1,index2;
        POINT p3;

        index1 = NodeList.size();
        for(jk=0; jk<NodeList.size(); jk++)
        {
            p3 = NodeList[jk];
            if(CanMerge(&p3, &p1, &Fuzzy))
            {
                index1 = jk;
                break;
            }
        }
        if(index1 == NodeList.size())
        {	
            NodeList.push_back(p1);
        }

        index2 = NodeList.size();
        for(jk=0; jk<NodeList.size(); jk++)
        {
            p3 = NodeList[jk];
            if(CanMerge(&p3, &p2, &Fuzzy))
            {
                index2 = jk;
                break;
            }
        }
        if(index2 == NodeList.size())
        {	
            NodeList.push_back(p2);
        }

        if(index1 != index2)
        {
            LineScript ls;
            ls.cost = plink.m_iTotal;// * -1;
            ls.LinkPos = ii;
            ls.stindex = index1;
            ls.edindex = index2;
            LineList.push_back(ls);
        }
    }//
    int nodessize = NodeList.size();
    if(nodessize < 1)
    {
        LineList.clear();
        NodeList.clear();
        return;
    }
    LineScript** pLineScripts;
    pLineScripts  = new LineScript*[nodessize];
    for (int j=0; j<nodessize; j++)
    {
        pLineScripts[j] = new LineScript[nodessize];
    }

    for (int j=0; j<LineList.size(); j++)
    {
        LineScript ls = LineList[j];
        if(ls.edindex < nodessize && ls.edindex > -1 && 
            ls.stindex > -1 && ls.stindex < nodessize &&
            (ls.cost < pLineScripts[ls.edindex][ls.stindex].cost || 
            ls.cost < pLineScripts[ls.stindex][ls.edindex].cost) )
        {
            pLineScripts[ls.edindex][ls.stindex] = ls;
            pLineScripts[ls.stindex][ls.edindex] = ls;
        }
    }

    //计算最长路径
    LineScript pMaxPath;
    pMaxPath.totalCost = 0;
    for(int i=0; i<nodessize; i++)
    {
        for(int j=0; j<nodessize; j++)
        {
            pLineScripts[i][j].totalCost = pLineScripts[i][j].cost;
            if ( (i != j) && !(pLineScripts[i][j].totalCost > CMP_INT) )
            {
                pLineScripts[i][j].path = i ; //记录前驱位置

                if(pMaxPath.totalCost <  pLineScripts[i][j].totalCost)
                {
                    pMaxPath = pLineScripts[i][j];
                    pMaxPath.stindex = i;
                    pMaxPath.edindex = j;
                }
            }
            else
            {
                pLineScripts[i][j].path = -1 ; 
            }
        }
    }
    for(int k=0; k<nodessize; k++) 
    {
        for(int i=0; i<nodessize; i++)
        {
            if(k == i || pLineScripts[i][k].totalCost > CMP_INT)
            {
                continue;
            }

            for(int j=0; j<nodessize; j++)
            {
                if(j==k || i==j || pLineScripts[k][j].totalCost  > CMP_INT)
                {
                    continue;
                }
                if(pLineScripts[i][k].totalCost + pLineScripts[k][j].totalCost < pLineScripts[i][j].totalCost)
                {
                    pLineScripts[i][j].totalCost = pLineScripts[i][k].totalCost + pLineScripts[k][j].totalCost;
                    pLineScripts[i][j].path = pLineScripts[k][j].path;

                    if(pMaxPath.totalCost <  pLineScripts[i][j].totalCost)
                    {
                        pMaxPath = pLineScripts[i][j];
                        pMaxPath.stindex = i;
                        pMaxPath.edindex = j;
                    }
                }
            }//
        }
    }
#if 0
    TRACE("\n****************totalCost\n");
    for(int k=0; k<nodessize; k++) 
    {
        TRACE(" %5d", k);
    }
    for(int k=0; k<nodessize; k++) 
    {
        TRACE("\n");
        for(int i=0; i<nodessize; i++)
        {
            TRACE(" %5d", pLineScripts[k][i].totalCost);
        }
    }
    TRACE("\n****************cost\n");

    for(int k=0; k<nodessize; k++) 
    {
        TRACE(" %5d", k);
    }
    for(int k=0; k<nodessize; k++) 
    {
        TRACE("\n");
        for(int i=0; i<nodessize; i++)
        {
            TRACE(" %5d", pLineScripts[k][i].cost);
        }
    }
    TRACE("\n****************path\n");
    for(int k=0; k<nodessize; k++) 
    {
        TRACE(" %5d", k);
    }
    for(int k=0; k<nodessize; k++) 
    {
        TRACE("\n");
        for(int i=0; i<nodessize; i++)
        {
            TRACE(" %5d", pLineScripts[k][i].path);
        }
    }
    TRACE("****************\n");
#endif 
    //搜索路径节点
    int* TempPath = new int[nodessize*2];
    if(pMaxPath.edindex > -1 && pMaxPath.stindex > -1)
    {
        bool bFail = false;
        int i = pMaxPath.edindex;
        int k = 0;
        int j;
        TempPath[k] = pMaxPath.edindex;
        k++;
        while(i != pMaxPath.stindex)
        {
            if(k > 2*nodessize-1)
            {
                bFail = true;
                break;
            }

            j = pLineScripts[pMaxPath.stindex][i].path;
            if(j<0)
            {
                bFail = true;
                break;
            }
            //环路处理
            if(pLineScripts[j][i].totalCost > CMP_INT )
            {
                TempPath[k] = -1;
                k++;
            }

            if(k > 2*nodessize-1)
            {
                bFail = true;
                break;
            }

            TempPath[k] = j;
            k++;

            i = j;
        } //end while

        if(bFail == false)
        {
            if(k > 0)
            {
                int pntCount = 0;
                for(j=k-1; j>0; j--)
                {
                    if( pLineScripts[TempPath[j]][TempPath[j-1]].LinkPos > -1 && pLineScripts[TempPath[j]][TempPath[j-1]].LinkPos<pLink->size() )
                    {
                        stLinkPart plink1 = pLink->at(pLineScripts[TempPath[j]][TempPath[j-1]].LinkPos);
                        pntCount += plink1.m_iPointNum;
                    }
                }
                TRACE("\n pntCount=%d  k=%d\n", pntCount, k);
                if(pntCount > 0)
                {
                    pLinkPart->m_pLinkPtr = new POINT[pntCount];
                    pLinkPart->m_iPointNum = pntCount;
                    *iRetLenght = pntCount;
                    stLinkPart plink1;
                    stLinkPart plink2;
                    LineScript ls1;
                    LineScript ls2;
                    int index1 = -1;
                    int inc1 =0;
                    if( k > 2 )
                    {
                        ls1 = pLineScripts[TempPath[k-1]][TempPath[k-2]];
                        if(ls1.LinkPos > -1 && ls1.LinkPos < pLink->size())
                        {
                            plink1 = pLink->at(ls1.LinkPos);
                        }

                        for(j=k-2; j>0; j--)
                        {
                            ls2 = pLineScripts[TempPath[j]][TempPath[j-1]];
                            if( ls2.LinkPos > -1 && ls2.LinkPos<pLink->size() )
                            {
                                plink2 = pLink->at(ls2.LinkPos);
                                if(index1 == -1)
                                {
                                    if(ls2.stindex == ls1.stindex)
                                    {
                                        index1 = ls2.edindex;

                                        for (int kk=plink1.m_iPointNum-1; kk>-1; kk--,inc1++)
                                        {
                                            pLinkPart->m_pLinkPtr[inc1] = plink1.m_pLinkPtr[kk];
                                        }
                                        for (int kk=0; kk<plink2.m_iPointNum; kk++,inc1++)
                                        {
                                            pLinkPart->m_pLinkPtr[inc1] = plink2.m_pLinkPtr[kk];
                                        }
                                    }
                                    else if(ls2.stindex == ls1.edindex)
                                    {
                                        index1 = ls2.edindex;

                                        for (int kk=0; kk<plink1.m_iPointNum; kk++,inc1++)
                                        {
                                            pLinkPart->m_pLinkPtr[inc1] = plink1.m_pLinkPtr[kk];
                                        }
                                        for (int kk=0; kk<plink2.m_iPointNum; kk++,inc1++)
                                        {
                                            pLinkPart->m_pLinkPtr[inc1] = plink2.m_pLinkPtr[kk];
                                        }
                                    }
                                    else if(ls2.edindex == ls1.edindex)
                                    {
                                        index1 = ls2.stindex;

                                        for (int kk=0; kk<plink1.m_iPointNum; kk++,inc1++)
                                        {
                                            pLinkPart->m_pLinkPtr[inc1] = plink1.m_pLinkPtr[kk];
                                        }
                                        for (int kk=plink2.m_iPointNum-1; kk>-1; kk--,inc1++)
                                        {
                                            pLinkPart->m_pLinkPtr[inc1] = plink2.m_pLinkPtr[kk];
                                        }
                                    }
                                    else if(ls2.edindex == ls1.stindex)
                                    {
                                        index1 = ls2.stindex;

                                        for (int kk=plink1.m_iPointNum-1; kk>-1; kk--,inc1++)
                                        {
                                            pLinkPart->m_pLinkPtr[inc1] = plink1.m_pLinkPtr[kk];
                                        }
                                        for (int kk=plink2.m_iPointNum-1; kk>-1; kk--,inc1++)
                                        {
                                            pLinkPart->m_pLinkPtr[inc1] = plink2.m_pLinkPtr[kk];
                                        }
                                    }
                                    else
                                    {

                                    }
                                    ls1 = ls2;
                                    plink1 = plink2;
                                }
                                else
                                {
                                    if(ls2.stindex == index1)
                                    {
                                        index1 = ls2.edindex;
                                        for (int kk=0; kk<plink2.m_iPointNum; kk++,inc1++)
                                        {
                                            pLinkPart->m_pLinkPtr[inc1] = plink2.m_pLinkPtr[kk];
                                        }
                                    }
                                    else if(ls2.edindex == index1)
                                    {
                                        index1 = ls2.stindex;
                                        for (int kk=plink2.m_iPointNum-1; kk>0; kk--,inc1++)
                                        {
                                            pLinkPart->m_pLinkPtr[inc1] = plink2.m_pLinkPtr[kk];
                                        }
                                    }
                                    else
                                    {
                                        index1 = -1;
                                        inc1=0;
                                    }
                                    ls1 = ls2;
                                    plink1 = plink2;
                                }
                            }
                        }//for
                    }
                    else
                    {
                        ls1 = pLineScripts[TempPath[k-1]][TempPath[k-2]];
                        if(ls1.LinkPos > -1 && ls1.LinkPos < pLink->size())
                        {
                            plink1 = pLink->at(ls1.LinkPos);
                            for (int kk=plink1.m_iPointNum-1; kk>-1; kk--,inc1++)
                            {
                                pLinkPart->m_pLinkPtr[inc1] = plink1.m_pLinkPtr[kk];
                            }
                        }
                    }
                    pLinkPart->m_iPointNum = inc1;
                    *iRetLenght = inc1;
                    int iRet = 0;
                    PolySimplifyInt2D(2.0, pLinkPart->m_pLinkPtr, pLinkPart->m_iPointNum, &iRet);
                    pLinkPart->m_iPointNum = iRet;
                    *iRetLenght = iRet;
                }
            }
        }
    }

    for (int j=0; j<nodessize; j++)
    {
        delete []pLineScripts[j];
    }
    delete []pLineScripts;
    pLineScripts = NULL;

    delete []TempPath;
    TempPath = NULL;

    LineList.clear();
    NodeList.clear();
}

void CLabelAdjust::CreateRect(POINT& centerPnt, RECT& r ,int& width)
{
    //以某点为中心产生一个矩形
    r.left = centerPnt.x - width  / 2;
    r.right = r.left + width;
    r.top = centerPnt.y - width / 2;
    r.bottom = r.top + width;
}

void CLabelAdjust::CreateRect(POINT& centerPnt, RECT& r ,int width, int height)
{
    r.left = centerPnt.x - width  / 2;
    r.right = r.left + width;
    r.top = centerPnt.y - height / 2;
    r.bottom = r.top + height;
}

void CLabelAdjust::GetRECT(stLabelPOI& LabelPOI, UINT LabelDirect, RECT& rcDis, BOOL bHasArrow, int nIconWidth, int nIconHeight, int nStrLen )
{
    //根据标注方位取区域
    int arrowLen = ICON_TEXT_SPACE;
    if(bHasArrow)
    {
        arrowLen = ARROW_LENGTH;
    }

    // 20110408-liujingzhou 字符过长时采用多行处理方式
    // 当字符串超过5个字符时采用换行处理
    BOOL bMultiLine = FALSE;
    int m = 0;
    int nCharWidth = 0;
    if ( nStrLen > 0 )
    {
        // 全角/半角，容易出现问题，直接采用14固定值
        nCharWidth = 14; // LabelPOI.m_Size.cx / nStrLen;
        if ( nStrLen > 5 )
        {
            m = (nStrLen % 2 == 0) ? (nStrLen/2) : (nStrLen/2+1);
            bMultiLine = TRUE;
        }
    }

    switch(LabelDirect)
    {
    case Right:
        {
            rcDis.left = LabelPOI.m_LabelPnt.x + nIconWidth / 2 + arrowLen;
            rcDis.top = LabelPOI.m_LabelPnt.y - LabelPOI.m_Size.cy / 2;

            if ( bMultiLine )
            {
                rcDis.right = rcDis.left + nCharWidth * m;
                rcDis.bottom =  rcDis.top + LabelPOI.m_Size.cy * 2;
            }
            else
            {
                rcDis.right = rcDis.left + LabelPOI.m_Size.cx;
                rcDis.bottom =  rcDis.top + LabelPOI.m_Size.cy;
            }
            break;
        }
    case Left:
        {
            rcDis.right = LabelPOI.m_LabelPnt.x - nIconWidth/2 - arrowLen;
            rcDis.top = LabelPOI.m_LabelPnt.y - LabelPOI.m_Size.cy / 2;

            if ( bMultiLine )
            {
                rcDis.left = rcDis.right - nCharWidth * m;
                rcDis.bottom =  rcDis.top + LabelPOI.m_Size.cy * 2;
            }
            else
            {
                rcDis.left = rcDis.right - LabelPOI.m_Size.cx;
                rcDis.bottom =  rcDis.top + LabelPOI.m_Size.cy;
            }
            break;
        }
    case Top:
        {
            rcDis.left = LabelPOI.m_LabelPnt.x - LabelPOI.m_Size.cx / 2;
            rcDis.top = LabelPOI.m_LabelPnt.y - nIconHeight/2 - LabelPOI.m_Size.cy - arrowLen;
            rcDis.right = rcDis.left + LabelPOI.m_Size.cx;
            rcDis.bottom =  rcDis.top + LabelPOI.m_Size.cy;
            break;
        }
    case Bottom:
        {
            rcDis.left = LabelPOI.m_LabelPnt.x - LabelPOI.m_Size.cx / 2;
            rcDis.top = LabelPOI.m_LabelPnt.y + nIconHeight/2 + arrowLen;
            rcDis.right = rcDis.left + LabelPOI.m_Size.cx;
            rcDis.bottom =  rcDis.top + LabelPOI.m_Size.cy;
            break;
        }
    }
}

int CLabelAdjust::GetTextAlign(int d)
{
    // 采用多行时，DT_WORDBREAK 有效，但Top和Bottom则很难处理
    // ???
    int align = 0;
    switch(d)
    {
    case Right:
        {
            align = DT_LEFT|DT_NOPREFIX|DT_WORDBREAK;
            break;
        }

    case Top:
        {
            align = DT_BOTTOM|DT_SINGLELINE|DT_NOPREFIX;
            break;
        }

    case Left:
        {
            align = DT_RIGHT|DT_NOPREFIX|DT_WORDBREAK;
            break;
        }

    case Bottom:
        {
            align = DT_TOP|DT_SINGLELINE|DT_NOPREFIX;
            break;
        }
    }
    return align;
}

void CLabelAdjust::PullOther(stLabelPOI& LabelPOI, UINT LabelDirect,BOOL bDrawArrow, int nIconWidth, int nIconHeight, int nStrLen)
{
    //将末注记的区域出栈
    RECT rcDis;
    for(int i=0; i<LABEL_DIRECTS_ENUM_COUNT; i++)
    {
        if(m_PriorityMatrix[i] == LabelDirect || (LabelPOI.m_LabelDirect & m_PriorityMatrix[i]) != m_PriorityMatrix[i])
        {
            continue;
        }
        GetRECT(LabelPOI, m_PriorityMatrix[i], rcDis, bDrawArrow, nIconWidth, nIconHeight, nStrLen);
        Pull(rcDis);
    }
}

BOOL CLabelAdjust::Push(RECT& r)
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

void CLabelAdjust::Pull(RECT& r)
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

void CLabelAdjust::RectArrow(HDC hDC, RECT& r, UINT LabelDirect)
{
    //画带指向的箭头
    //指向标记赋值
    POINT pnt[4];
    int iArrow2Top = (r.bottom - r.top - ARROW_WIDTH) >>1;
    int iArrow2Left = (r.right - r.left - ARROW_WIDTH) >>1;

    switch(LabelDirect)
    {
    case Right:
        pnt[0].x = r.left - ARROW_LENGTH;
        pnt[0].y = (r.top + r.bottom)>>1;
        pnt[1].x = r.left;
        pnt[1].y = r.top + iArrow2Top;
        pnt[2].x = r.left;
        pnt[2].y = r.top + iArrow2Top + ARROW_WIDTH;
        pnt[3].x = r.left - ARROW_LENGTH;
        pnt[3].y = (r.top + r.bottom)>>1;
        break;

    case Top:
        pnt[0].x = (r.left + r.right)>>1;
        pnt[0].y = r.bottom + ARROW_LENGTH;
        pnt[1].x = r.left + iArrow2Left;
        pnt[1].y = r.bottom;
        pnt[2].x = r.left + iArrow2Left + ARROW_WIDTH;
        pnt[2].y = r.bottom;
        pnt[3].x = (r.left + r.right)>>1;
        pnt[3].y = r.bottom + ARROW_LENGTH;
        break;

    case Left:
        pnt[0].x = r.right + ARROW_LENGTH;
        pnt[0].y = (r.top + r.bottom)>>1;
        pnt[1].x = r.right;
        pnt[1].y = r.top + iArrow2Top;
        pnt[2].x = r.right;
        pnt[2].y = r.top + iArrow2Top + ARROW_WIDTH;
        pnt[3].x = r.right + ARROW_LENGTH;
        pnt[3].y = (r.top + r.bottom)>>1;
        break;

    case Bottom:
        pnt[0].x = (r.left + r.right)>>1;
        pnt[0].y = r.top - ARROW_LENGTH;
        pnt[1].x = r.left + iArrow2Left;
        pnt[1].y = r.top;
        pnt[2].x = r.left + iArrow2Left + ARROW_WIDTH;
        pnt[2].y = r.top;
        pnt[3].x = (r.left + r.right)>>1;
        pnt[3].y =  r.top - ARROW_LENGTH;
        break;
    }

    HBRUSH hCmpOldBr = (HBRUSH)SelectObject(hDC, m_hArrowBrush);
    HPEN pOldPen = (HPEN)SelectObject(hDC, m_hArrowPen);
    ::Polygon(hDC, pnt, 4);
    SelectObject(hDC, pOldPen);
    SelectObject(hDC, hCmpOldBr);
}

void CLabelAdjust::DrawFrame(HDC hDC, RECT& r, int type )
{
    //画注记外框
    if(type == 0)
    {
        POINT pnt[5];
        //矩形赋值
        pnt[0].x = r.left;
        pnt[0].y = r.top-2;

        pnt[1].x = r.right;
        pnt[1].y = r.top-2;

        pnt[2].x = r.right;
        pnt[2].y = r.bottom;

        pnt[3].x = r.left;
        pnt[3].y = r.bottom;

        pnt[4].x = r.left;
        pnt[4].y = r.top-2;
        ::Polyline(hDC, pnt, 5);
    }
    else if(type == 1)
    {
        ::Ellipse(hDC, r.left, r.top-2, r.right, r.bottom);
    }
    else
    {

    }
}

void CLabelAdjust::BackupMatrix(BOOL T)
{
	if(T)
	{
		for(int i=0; i<m_nHeight; i++)
		{
			memcpy(m_iMatrix2[i], m_iMatrix[i], m_nWidth);			
		}
	}
	else
	{
		for(int i=0; i<m_nHeight; i++)
		{
			memcpy(m_iMatrix[i], m_iMatrix2[i], m_nWidth);			
		}
	}
}

void CLabelAdjust::DrawLineAlphabetName( CDisplayInfo* pDisplayInfo, CCanvas* pCanvas, TCHAR* szName, vector<stLinkPart>* pLink, CGeoTextStyle* pTextStyle )
{
	SIZE sz;
	HDC hDC = pCanvas->GetDC();

    vector<stLabelLine> stLabelLineList;
	stLinkPart pstLinkPart; 
	int iRetNodesNum,i;
	int dis = 0;
	for (int k=0; k<pLink->size(); k++)
	{
		pstLinkPart = pLink->at(k);
		dis += pstLinkPart.m_iTotal;
	}

	if(GetTextExtentPoint32(hDC, szName, wcslen(szName), &sz))
	{
		sz.cx += OFF_CX;
		sz.cy += OFF_CY;
	}

	if(dis < sz.cy)
	{
#ifdef _DEBUG
		TRACE(L"dis < stringwide");
		TRACE("\n");
#endif
		return;
	}

	//多线合一
	double Fuzzy = 2;
	MultiLines2OneLineEx(pLink, &pstLinkPart,&iRetNodesNum,Fuzzy);
	pstLinkPart.m_iPointNum = iRetNodesNum;
#ifdef DRAW_DEBUG_LINE
	GDIColor color;

	color.a = 0;
	color.r = GetRValue(RGB(255,0,0));
	color.g = GetGValue(RGB(255,0,0));
	color.b = GetBValue(RGB(255,0,0));

	GDIPen FillPen1;
	FillPen1.flags    = GDIPEN_WIDTH|GDIPEN_COLOR|GDIPEN_STARTLINECAP|GDIPEN_ENDLINECAP|GDIPEN_LINEJOIN;
	FillPen1.width    = 5;
	FillPen1.color    = color;
	FillPen1.startCap = Round;
	FillPen1.endCap   =Round;
	FillPen1.lineJoin = RoundX;
	GDIPen oldpen = pCanvas->SetGdiPen(&FillPen1);
	Polyline(hDC, pstLinkPart.m_pLinkPtr, pstLinkPart.m_iPointNum);
	pCanvas->SetGdiPen(&oldpen);
#endif
	if(pstLinkPart.m_iPointNum < 2)
	{
		if(pstLinkPart.m_pLinkPtr != NULL)
		{
			delete [](pstLinkPart.m_pLinkPtr);
			pstLinkPart.m_pLinkPtr = NULL;
			pLink->at(pLink->size()-1).m_pLinkPtr = NULL;
		}
		pstLinkPart.m_iPointNum = 0;
		return;
	}

#if 0
	//去重点
	int nodes = Vect_line_prune(&pstLinkPart, iRetNodesNum);
	pstLinkPart.m_iPointNum = nodes;
	iRetNodesNum = nodes;
	if(iRetNodesNum < 2)
	{
		if(pstLinkPart.m_pLinkPtr != NULL)
		{
			delete [](pstLinkPart.m_pLinkPtr);
			pstLinkPart.m_pLinkPtr = NULL;
		}
		pstLinkPart.m_iPointNum = 0;
		::SetTextColor( hDC, crold);
		::SetBkColor(hDC, crBkOld);
		::SelectObject(hDC, holdfont);
		return;
	}
#endif

	//搜索注记位置
	double dx,dy,length;
	float totalDis,lastDis,t1;
	BOOL bContinue = TRUE;
	stLabelLine LabelLineInfo;
	POINT pt;
	RECT r;
	double iNoteNum;

	lastDis = 0;//上次剩余长度
	totalDis =0;//累加长度

	//TRACE(L"\n搜索注记位置\n");
	for (i=0; i<iRetNodesNum-1; i++)
	{
		vect(&pstLinkPart.m_pLinkPtr[i], &pstLinkPart.m_pLinkPtr[i+1],&dx,&dy, &length);
		//TRACE(L"\length:%d\n", (int)length);

		totalDis += length;//累加长度

		LabelLineInfo.iCanLabelNum = 0;
		LabelLineInfo.bContinuePre = bContinue;
		LabelLineInfo.fDx = dx;
		LabelLineInfo.fDy = dy;
		LabelLineInfo.fToPreDis = totalDis;//和前面距离累加
		LabelLineInfo.flength = length;
		LabelLineInfo.iIndex = i;
		LabelLineInfo.fStartlen = 0;
		LabelLineInfo.dAng = 0;

		lastDis = length;//开始一个注记的可用长度
		iNoteNum = 0;

		while (lastDis > 0)
		{
			//TRACE(L"\nLastDis:%d\n", (int)lastDis);
			t1 = LabelLineInfo.fStartlen + iNoteNum * sz.cy;
			pt.x = pstLinkPart.m_pLinkPtr[i].x + t1 * dx;
			pt.y = pstLinkPart.m_pLinkPtr[i].y + t1 * dy;
			CreateRect(pt,r, sz.cx, sz.cy);
			short val = CanLabel(r);

			if(val > BAN_VALUE)
			{
				if(LabelLineInfo.iCanLabelNum >= 0 )
				{
					LabelLineInfo.fEndLen = t1 - sz.cy / 2;
					stLabelLineList.push_back(LabelLineInfo);
				}

				bContinue = FALSE;

				//开始一个新的搜寻
				LabelLineInfo.iCanLabelNum =0;
				LabelLineInfo.bContinuePre = bContinue;
				LabelLineInfo.fDx = dx;
				LabelLineInfo.fDy = dy;
				LabelLineInfo.fToPreDis = totalDis;
				LabelLineInfo.flength = length;
				LabelLineInfo.iIndex = i;
				LabelLineInfo.fStartlen = t1 + sz.cy/2;
				LabelLineInfo.dAng = 0;
				iNoteNum = 0;
			}
			else
			{
				LabelLineInfo.iCanLabelNum++;
				bContinue = TRUE;
				iNoteNum ++;
			}
			lastDis -= sz.cy;
		}//while

		if(LabelLineInfo.fStartlen < LabelLineInfo.flength )
		{
			LabelLineInfo.fEndLen = length;
			stLabelLineList.push_back(LabelLineInfo);
		}
	}

	if(totalDis <  sz.cy || stLabelLineList.size() == 0)
	{
#ifdef _DEBUG
		TRACE(L"totalDis < stringwide || stLabelLineList.size() == 0");
		TRACE("\n");
#endif
		stLabelLineList.clear();
		if(pstLinkPart.m_pLinkPtr != NULL)
		{
			delete [](pstLinkPart.m_pLinkPtr);
			pstLinkPart.m_pLinkPtr = NULL;
		}
		pstLinkPart.m_iPointNum = 0;
		return;	
	}

	//求最佳注记位置
	//TRACE(L"\n求最佳注记位\n");
	double offset = (totalDis-sz.cy) / 2.0;
	vector<LineStatInfo> stLineStatInfoList;
	LineStatInfo stLineStatInfo;
	stLabelLine pstLabelLine;
	short int iStart=0;
	short int iEnd=-1;
	short int iCanLabelNum = 0;
	float fStartDis = 0;
	float fEnddis = 0;
	float rat = 0;
	int iLabelNums = 0;

	pstLabelLine = stLabelLineList.at(0);
	fStartDis = pstLabelLine.fStartlen;

	for (i=0; i<stLabelLineList.size(); i++)
	{
		pstLabelLine = stLabelLineList.at(i);
		iLabelNums += pstLabelLine.iCanLabelNum;

		if(pstLabelLine.bContinuePre == FALSE)
		{
			if(iStart <= iEnd  && fEnddis-fStartDis > sz.cy)
			{
				//rat值越小越好
				if((fStartDis - offset) * (offset - fEnddis) <= 0 )
				{
					rat = 0;
				}
				else
				{
					rat = fEnddis - fStartDis;
				}
				stLineStatInfo.fHalfRate = rat;
				stLineStatInfo.iStartIndex = iStart;
				stLineStatInfo.iEndIndex = iEnd;
				stLineStatInfo.iLabelNums = iLabelNums;
				stLineStatInfoList.push_back(stLineStatInfo);
			}
			//下一循环
			fStartDis = pstLabelLine.fToPreDis - pstLabelLine.flength + pstLabelLine.fStartlen;
			iStart = i;
			iLabelNums = 0;
		}
		iEnd = i;
		fEnddis = pstLabelLine.fEndLen + pstLabelLine.fToPreDis - pstLabelLine.flength;
	}

	if(iStart <= iEnd  && fEnddis-fStartDis > sz.cy)
	{
		//rat值越小越好
		if((fStartDis - offset) * (offset - fEnddis) <= 0 )
		{
			rat = 0;
		}
		else
		{
			rat = fEnddis - fStartDis;
		}
		stLineStatInfo.fHalfRate = rat;
		stLineStatInfo.iStartIndex = iStart;
		stLineStatInfo.iEndIndex = iEnd;
		stLineStatInfo.iLabelNums = iLabelNums;
		stLineStatInfoList.push_back(stLineStatInfo);
	}
	//排序，标注
	//TRACE(L"\n排序，标注\n");
	if (stLineStatInfoList.size() > 0)
	{
		BOOL bFollowLine = TRUE;
		qsort(&(stLineStatInfoList[0]), stLineStatInfoList.size(), sizeof(LineStatInfo), compareEx);
		stLineStatInfo = stLineStatInfoList[0];
		//TRACE("\n ang = %f \n", stLineStatInfo.dAng);
		int angle = (int(stLineStatInfo.dAng + 360)) % 360;
		iStart = stLineStatInfo.iStartIndex;
		iEnd = stLineStatInfo.iEndIndex;
		stLabelLine lli;
		stLabelLine lli1;
		lli = stLabelLineList.at(iStart);
		lli1 = stLabelLineList.at(iEnd);

		//可注记长度
		t1 = lli1.fToPreDis - lli1.flength + lli1.fEndLen - lli.fToPreDis + lli.flength - lli.fStartlen;
		if(t1 <  sz.cy)
		{
			stLineStatInfoList.clear();
			stLabelLineList.clear();
			if(pstLinkPart.m_pLinkPtr != NULL)
			{
				delete [](pstLinkPart.m_pLinkPtr);
				pstLinkPart.m_pLinkPtr = NULL;
			}
			pstLinkPart.m_iPointNum = 0;
#ifdef _DEBUG
			TRACE(L"t1 < fontwidth * iTextLen");
			TRACE("\n");
#endif
			return;
		}

		//偏移和间距
		offset = (t1 -  sz.cy) / 2.0;

		float dis = 0;
		float offdis = offset;
		RECT r1;
		COLORREF clr =  GetTextColor(hDC); 
		for (i=iStart; i<=iEnd;)
		{
			lli = stLabelLineList[i];
			t1 = lli.fEndLen - lli.fStartlen;
			//取下一条线注记
			if(offdis > dis + t1)
			{
				dis += t1;
				i++;
				continue;
			}

			pt.x = pstLinkPart.m_pLinkPtr[lli.iIndex].x + (lli.fStartlen + offdis - dis) * lli.fDx;
			pt.y = pstLinkPart.m_pLinkPtr[lli.iIndex].y + (lli.fStartlen + offdis - dis) * lli.fDy;
			CreateRect(pt,r1, sz.cx, sz.cy);

            // 背景
			pCanvas->Ellipse(pTextStyle->m_FillPen1, (GDIRect)r1);
            // 外边框
			pCanvas->Ellipse(pTextStyle->m_FillPen2, (GDIRect)r1);

            // 写字
			CreateRect(pt,r, (int)sz.cx-OFF_CX, (int)sz.cy-OFF_CY);
			if( !((pTextStyle->m_drawtype&eTEXT_DT_OUTLINEFONT) > 0) )
			{
				::DrawText(hDC, szName, wcslen(szName), &r, DT_WORDBREAK);
			}
			else
			{
				pCanvas->DrawEdgeText(szName, wcslen(szName), &r, pTextStyle->m_clrFont, pTextStyle->m_clrBack);
			}

			AddBan(r1, BAN_LINENAME_JUDGE_VALUE);
			break;
		}
		TRACE("注记完成");
		TRACE("\n");
	}

	//TRACE(L"\nEND注记\n");
	//释放空间
	stLineStatInfoList.clear();
	stLabelLineList.clear();
	if(pstLinkPart.m_pLinkPtr != NULL)
	{
		delete [](pstLinkPart.m_pLinkPtr);
		pstLinkPart.m_pLinkPtr = NULL;
	}
	pstLinkPart.m_iPointNum = 0;
}

void CLabelAdjust::DrawLineChineseName( CDisplayInfo* pDisplayInfo, CCanvas* pCanvas, TCHAR* szName, vector< stLinkPart >* pLink, CGeoTextStyle* pTextStyle )
{
    //绘制线注记

    //得到字体大小	
    SIZE sz;
    int iFontHeight = 16;
    HDC hDC = pCanvas->GetDC();

    if(GetTextExtentPoint32(hDC, L"国", 1, &sz))
    {
        iFontHeight = sz.cy;
    }

    int iTextLen = wcslen(szName);
    vector<stLabelLine> stLabelLineList;
    stLinkPart pstLinkPart; 
    int iRetNodesNum,i;
    int dis = 0;
    for (int k=0; k<pLink->size(); k++)
    {
        pstLinkPart = pLink->at(k);
        dis += pstLinkPart.m_iTotal;
    }

    int stringwide = iFontHeight*iTextLen;
    if(GetTextExtentPoint32(hDC, szName, iTextLen, &sz))
    {
        stringwide = sz.cx;
    }

    if(dis < stringwide)
    {
#ifdef _DEBUG
        TRACE(L"dis < stringwide");
        TRACE("\n");
#endif
        return;
    }

    //多线合一
    double Fuzzy = 2;
    MultiLines2OneLineEx(pLink, &pstLinkPart,&iRetNodesNum,Fuzzy);
    pstLinkPart.m_iPointNum = iRetNodesNum;
#ifdef DRAW_DEBUG_LINE
    GDIColor color;

    color.a = 0;
    color.r = GetRValue(RGB(255,0,0));
    color.g = GetGValue(RGB(255,0,0));
    color.b = GetBValue(RGB(255,0,0));

    GDIPen FillPen1;
    FillPen1.flags    = GDIPEN_WIDTH|GDIPEN_COLOR|GDIPEN_STARTLINECAP|GDIPEN_ENDLINECAP|GDIPEN_LINEJOIN;
    FillPen1.width    = 5;
    FillPen1.color    = color;
    FillPen1.startCap = Round;
    FillPen1.endCap   =Round;
    FillPen1.lineJoin = RoundX;
    GDIPen oldpen = pCanvas->SetGdiPen(&FillPen1);
    Polyline(hDC, pstLinkPart.m_pLinkPtr, pstLinkPart.m_iPointNum);
    pCanvas->SetGdiPen(&oldpen);
#endif
    if(pstLinkPart.m_iPointNum < 2)
    {
        if(pstLinkPart.m_pLinkPtr != NULL)
        {
            delete [](pstLinkPart.m_pLinkPtr);
            pstLinkPart.m_pLinkPtr = NULL;
        }
        pstLinkPart.m_iPointNum = 0;
        return;	
    }

    //去重点
#if 0
    int nodes = Vect_line_prune(&pstLinkPart, iRetNodesNum);
    pstLinkPart.m_iPointNum = nodes;
    iRetNodesNum = nodes;
    if(iRetNodesNum < 2)
    {
        if(pstLinkPart.m_pLinkPtr != NULL)
        {
            delete [](pstLinkPart.m_pLinkPtr);
            pstLinkPart.m_pLinkPtr = NULL;
        }
        pstLinkPart.m_iPointNum = 0;
        ::SetTextColor( hDC, crold);
        ::SetBkColor(hDC, crBkOld);
        ::SelectObject(hDC, holdfont);
        return;
    }
#endif

    //搜索注记位置
    double dx,dy,length,dPI1,dPI2,ang,dx1,dy1;
    float totalDis,lastDis,t1;
    float fontwidth,fonthalfwidth,fSpace;
    BOOL bContinue = TRUE;
    stLabelLine LabelLineInfo;
    POINT pt;
    RECT r;
    double iNoteNum;

    lastDis = 0;//上次剩余长度
    totalDis =0;//累加长度
    fontwidth = iFontHeight;
    fonthalfwidth = iFontHeight * 0.5;
    fSpace = fonthalfwidth / 2.0;
    dPI1 = 135;
    dPI2 = 315;
    dy1 = pstLinkPart.m_pLinkPtr[1].y-pstLinkPart.m_pLinkPtr[0].y;
    dx1 = pstLinkPart.m_pLinkPtr[1].x-pstLinkPart.m_pLinkPtr[0].x;
    double ang0 = (int(atan2(dy1, dx1) * 180.0 / PI +360)) % 360;

    //TRACE(L"\n搜索注记位置\n");
    for (i=0; i<iRetNodesNum-1; i++)
    {
        vect(&pstLinkPart.m_pLinkPtr[i], &pstLinkPart.m_pLinkPtr[i+1],&dx,&dy, &length);
        //TRACE(L"\length:%d\n", (int)length);
        dy1 = pstLinkPart.m_pLinkPtr[i+1].y-pstLinkPart.m_pLinkPtr[0].y;
        dx1 = pstLinkPart.m_pLinkPtr[i+1].x-pstLinkPart.m_pLinkPtr[0].x;
        ang = (int(atan2(dy1, dx1) * 180.0 / PI +360)) % 360;

        totalDis += length;//累加长度

        if( abs(ang-ang0)  < 150 )
        {
            bContinue = TRUE;
        }
        else
        {
            bContinue = FALSE;
        }
        LabelLineInfo.iCanLabelNum = 0;
        LabelLineInfo.bContinuePre = bContinue;
        LabelLineInfo.fDx = dx;
        LabelLineInfo.fDy = dy;
        LabelLineInfo.fToPreDis = totalDis;//和前面距离累加
        LabelLineInfo.flength = length;
        LabelLineInfo.iIndex = i;
        LabelLineInfo.fStartlen = 0;
        LabelLineInfo.dAng = ang;

        lastDis = length;//开始一个注记的可用长度
        iNoteNum = 0;

        while (lastDis > 0)
        {
            //TRACE(L"\nLastDis:%d\n", (int)lastDis);
            t1 = LabelLineInfo.fStartlen + iNoteNum * fontwidth;
            pt.x = pstLinkPart.m_pLinkPtr[i].x + t1 * dx;
            pt.y = pstLinkPart.m_pLinkPtr[i].y + t1 * dy;
            CreateRect(pt,r, iFontHeight);
            short val = CanLabel(r);

            if(val > BAN_VALUE)
            {
                if(LabelLineInfo.iCanLabelNum >= 0 )
                {
                    LabelLineInfo.fEndLen = t1 - fontwidth/2;
                    stLabelLineList.push_back(LabelLineInfo);
                }

                bContinue = FALSE;

                //开始一个新的搜寻
                LabelLineInfo.iCanLabelNum =0;
                LabelLineInfo.bContinuePre = bContinue;
                LabelLineInfo.fDx = dx;
                LabelLineInfo.fDy = dy;
                LabelLineInfo.fToPreDis = totalDis;
                LabelLineInfo.flength = length;
                LabelLineInfo.iIndex = i;
                LabelLineInfo.fStartlen = t1 + fontwidth/2;
                LabelLineInfo.dAng = ang;
                iNoteNum = 0;
            }
            else
            {
                LabelLineInfo.iCanLabelNum++;
                bContinue = TRUE;
                iNoteNum ++;
            }
            lastDis -= fontwidth;
        }//while

        if(LabelLineInfo.fStartlen < LabelLineInfo.flength )
        {
            LabelLineInfo.fEndLen = length;
            stLabelLineList.push_back(LabelLineInfo);
        }
        ang0 = ang;
    }

    if(totalDis < stringwide || stLabelLineList.size() == 0)
    {
#ifdef _DEBUG
        TRACE(L"totalDis < stringwide || stLabelLineList.size() == 0");
        TRACE("\n");
#endif
        // 		TCHAR szInfo[200];
        // 		wsprintf(szInfo,L"\n太短totalDis=%f ,fontwid=%f , size=%d\n", totalDis, fontwidth * iTextLen, stLabelLineList.size());
        // 		TRACE(szInfo);
        stLabelLineList.clear();
        if(pstLinkPart.m_pLinkPtr != NULL)
        {
            delete [](pstLinkPart.m_pLinkPtr);
            pstLinkPart.m_pLinkPtr = NULL;
        }
        pstLinkPart.m_iPointNum = 0;

        return;	
    }

    //求最佳注记位置
    //TRACE(L"\n求最佳注记位\n");
    double offset = totalDis / 2.0;
    vector<LineStatInfo> stLineStatInfoList;
    LineStatInfo stLineStatInfo;
    stLabelLine pstLabelLine;
    short int iStart=0;
    short int iEnd=-1;
    short int iCanLabelNum = 0;
    float fStartDis = 0;
    float fEnddis = 0;
    float rat = 0;
    int iLabelNums = 0;

    pstLabelLine = stLabelLineList.at(0);
    fStartDis = pstLabelLine.fStartlen;
    ang = 0;

    for (i=0; i<stLabelLineList.size(); i++)
    {
        pstLabelLine = stLabelLineList.at(i);
        ang += pstLabelLine.dAng * (pstLabelLine.fEndLen - pstLabelLine.fStartlen);
        iLabelNums += pstLabelLine.iCanLabelNum;

        if(pstLabelLine.bContinuePre == FALSE)
        {
            if(iStart <= iEnd  && fEnddis-fStartDis > fontwidth * iTextLen)
            {
                //rat值越小越好
                if((fStartDis - offset) * (offset - fEnddis) <= 0 )
                {
                    rat = 0;
                }
                else
                {
                    rat = fEnddis - fStartDis;
                }
                stLineStatInfo.dAng = ang / (fEnddis - fStartDis);
                stLineStatInfo.fHalfRate = rat;
                stLineStatInfo.iStartIndex = iStart;
                stLineStatInfo.iEndIndex = iEnd;
                stLineStatInfo.iLabelNums = iLabelNums;
                stLineStatInfoList.push_back(stLineStatInfo);
            }
            //下一循环
            fStartDis = pstLabelLine.fToPreDis - pstLabelLine.flength + pstLabelLine.fStartlen;
            iStart = i;
            ang = 0;
            iLabelNums = 0;
        }
        iEnd = i;
        fEnddis = pstLabelLine.fEndLen + pstLabelLine.fToPreDis - pstLabelLine.flength;
    }

    if(iStart <= iEnd  && fEnddis-fStartDis > fontwidth * iTextLen)
    {
        //rat值越小越好
        if((fStartDis - offset) * (offset - fEnddis) <= 0 )
        {
            rat = 0;
        }
        else
        {
            rat = fEnddis - fStartDis;
        }
        stLineStatInfo.fHalfRate = rat;
        stLineStatInfo.iStartIndex = iStart;
        stLineStatInfo.iEndIndex = iEnd;
        stLineStatInfo.iLabelNums = iLabelNums;
        stLineStatInfo.dAng = ang / (fEnddis - fStartDis) ;
        stLineStatInfoList.push_back(stLineStatInfo);
    }
    //排序，标注
    //TRACE(L"\n排序，标注\n");
    if (stLineStatInfoList.size() > 0)
    {
        BOOL bFollowLine = TRUE;
        qsort(&(stLineStatInfoList[0]), stLineStatInfoList.size(), sizeof(LineStatInfo), compareEx);
        stLineStatInfo = stLineStatInfoList[0];
        //TRACE("\n ang = %f \n", stLineStatInfo.dAng);
        int angle = (int(stLineStatInfo.dAng + 360)) % 360;
        if(angle < dPI1 || angle > dPI2)
        {
            bFollowLine = TRUE ;
        }
        else
        {
            bFollowLine = FALSE;
        }
        iStart = stLineStatInfo.iStartIndex;
        iEnd = stLineStatInfo.iEndIndex;
        stLabelLine lli;
        stLabelLine lli1;
        lli = stLabelLineList.at(iStart);
        lli1 = stLabelLineList.at(iEnd);

        //可注记长度
        t1 = lli1.fToPreDis - lli1.flength + lli1.fEndLen - lli.fToPreDis + lli.flength - lli.fStartlen;
        if(t1 < fontwidth * iTextLen)
        {
            stLineStatInfoList.clear();
            stLabelLineList.clear();
            if(pstLinkPart.m_pLinkPtr != NULL)
            {
                delete [](pstLinkPart.m_pLinkPtr);
                pstLinkPart.m_pLinkPtr = NULL;
            }
            pstLinkPart.m_iPointNum = 0;
#ifdef _DEBUG
            TRACE(L"t1 < fontwidth * iTextLen");
            TRACE("\n");
#endif
            return;	
        }

        //偏移和间距
        offset = (t1 - (fSpace + fontwidth) * iTextLen) / 2.0;
        if(offset < 0)
        {
            fSpace = (t1 - fontwidth*iTextLen) / iTextLen;
            offset = 0;
        }

        float dis;
        dis = 0;
        float offdis = offset;
        int kw = 0;//
        COLORREF clr =  GetTextColor(hDC); 
        for (i=iStart; i<=iEnd;)
        {
            if(kw >= iTextLen)
            {//注记完成
                break;
            }

            lli = stLabelLineList[i];
            t1 = lli.fEndLen - lli.fStartlen;
            //取下一条线注记
            if(offdis > dis + t1)
            {
                dis += t1;
                i++;
                continue;
            }

            pt.x = pstLinkPart.m_pLinkPtr[lli.iIndex].x + (lli.fStartlen + offdis - dis) * lli.fDx;
            pt.y = pstLinkPart.m_pLinkPtr[lli.iIndex].y + (lli.fStartlen + offdis - dis) * lli.fDy;
            CreateRect(pt,r, iFontHeight);

            if(bFollowLine == TRUE)
            {
                if( !((pTextStyle->m_drawtype&eTEXT_DT_OUTLINEFONT) > 0) )
                {
                    //SetTextColor(hDC, pTextStyle->m_clrFont);
                    ::DrawText(hDC, &szName[kw], 1, &r, DT_WORDBREAK);
                }
                else
                {
                    pCanvas->DrawEdgeText(&szName[kw], 1, &r, pTextStyle->m_clrFont, pTextStyle->m_clrBack);
                }
            }
            else
            {
                if( !((pTextStyle->m_drawtype&eTEXT_DT_OUTLINEFONT) > 0) )
                {
                    //SetTextColor(hDC, pTextStyle->m_clrFont);
                    ::DrawText(hDC, &szName[iTextLen-kw-1], 1, &r, DT_WORDBREAK);
                }
                else
                {
                    pCanvas->DrawEdgeText(&szName[iTextLen-kw-1], 1, &r, pTextStyle->m_clrFont, pTextStyle->m_clrBack);
                }
            }

            AddBan(r, BAN_LINENAME_JUDGE_VALUE);
            kw ++;
            offdis += (fontwidth + fSpace);
        }
        SetTextColor(hDC,clr);
        TRACE("注记完成");
        TRACE("\n");
    }

    //TRACE(L"\nEND注记\n");
    //释放空间
    stLineStatInfoList.clear();
    stLabelLineList.clear();
    if(pstLinkPart.m_pLinkPtr != NULL)
    {
        delete [](pstLinkPart.m_pLinkPtr);
        pstLinkPart.m_pLinkPtr = NULL;
    }
    pstLinkPart.m_iPointNum = 0;
}

void CLabelAdjust::DrawLineChineseNameEx( CDisplayInfo* pDisplayInfo, CCanvas* pCanvas, TCHAR* szName, vector< stLinkPart >* pLink, CGeoTextStyle* pTextStyle )
{
	//得到字体大小
	SIZE sz;
	int iFontHeight = 16;
	HDC hDC = pCanvas->GetDC();

	if(GetTextExtentPoint32(hDC, L"国", 1, &sz))
	{
		iFontHeight = sz.cy;
	}

	int iTextLen = wcslen(szName);
	vector<stLabelLine> stLabelLineList;
	stLinkPart pstLinkPart; 
	int iRetNodesNum,i;
	int dis = 0;
	for (int k=0; k<pLink->size(); k++)
	{
		pstLinkPart = pLink->at(k);
		dis += pstLinkPart.m_iTotal;
	}

	int stringwide = iFontHeight*iTextLen;
	if(GetTextExtentPoint32(hDC, szName, iTextLen, &sz))
	{
		stringwide = sz.cx;
	}

	if(dis < stringwide)
	{
#ifdef _DEBUG
		TRACE(L"dis < stringwide");
		TRACE("\n");
#endif
		return;
	}

	//多线合一
	double Fuzzy = 2;
	//typedef map<int,map<int,vector<int>>> arr;
	//arr arr2;
	//for (int ii=0; ii<pLink->size(); ii++)
	//{
	//	stLinkPart plink = pLink->at(ii);
	//	POINT p1 = plink.m_pLinkPtr[0];
	//	POINT p2 = plink.m_pLinkPtr[plink.m_iPointNum-1];
	//	TRACE("\n P1(%d,%d) P2(%d,%d)",p1.x, p1.y, p2.x,p2.y);
	//	arr2[p1.x][p1.y].push_back(ii);
	//}
	
	MultiLines2OneLineEx(pLink, &pstLinkPart,&iRetNodesNum,Fuzzy);
	pstLinkPart.m_iPointNum = iRetNodesNum;
	if(pstLinkPart.m_iPointNum < 2)
	{
		if(pstLinkPart.m_pLinkPtr != NULL)
		{
			delete [](pstLinkPart.m_pLinkPtr);
			pstLinkPart.m_pLinkPtr = NULL;
			pLink->at(pLink->size()-1).m_pLinkPtr = NULL;
		}
		pstLinkPart.m_iPointNum = 0;
		return;	
	}
	
#ifdef DRAW_DEBUG_LINE
	GDIColor color;

	color.a = 0;
	color.r = GetRValue(RGB(255,0,0));
	color.g = GetGValue(RGB(255,0,0));
	color.b = GetBValue(RGB(255,0,0));

	GDIPen FillPen1;
	FillPen1.flags    = GDIPEN_WIDTH|GDIPEN_COLOR|GDIPEN_STARTLINECAP|GDIPEN_ENDLINECAP|GDIPEN_LINEJOIN;
	FillPen1.width    = 5;
	FillPen1.color    = color;
	FillPen1.startCap = Round;
	FillPen1.endCap   =Round;
	FillPen1.lineJoin = RoundX;
	GDIPen oldpen = pCanvas->SetGdiPen(&FillPen1);
	Polyline(hDC, pstLinkPart.m_pLinkPtr, pstLinkPart.m_iPointNum);
	pCanvas->SetGdiPen(&oldpen);
#endif

	//搜索注记位置
	double dx,dy,length,dPI1,dPI2,ang,dx1,dy1, ang0;
	float totalDis,lastDis,t1;
	float fontwidth,fonthalfwidth,fSpace;
	BOOL bContinue = FALSE;
	stLabelLine LabelLineInfo;
	POINT pt;
	RECT r;
	double iNoteNum;

	lastDis = 0;//上次剩余长度
	totalDis =0;//累加长度
	fontwidth = iFontHeight;
	fonthalfwidth = iFontHeight * 0.5;
	fSpace = fonthalfwidth / 2.0;
	dPI1 = 135;
	dPI2 = 315;	

	BOOL bFollowLine = TRUE;
	BOOL bOldFollowLine = TRUE;
	vect(&pstLinkPart.m_pLinkPtr[0], &pstLinkPart.m_pLinkPtr[1],&dx,&dy, &length);
	ang = int(atan2(dy,dx) * 180 / PI + 360) % 360;

	if(ang <= dPI1 || ang >= dPI2)
	{
		bOldFollowLine = TRUE;
	}
	else
	{
		bOldFollowLine = FALSE;
	}

	//TRACE(L"\n搜索注记位置\n");
	for (i=0; i<iRetNodesNum-1; i++)
	{
		vect(&pstLinkPart.m_pLinkPtr[i], &pstLinkPart.m_pLinkPtr[i+1],&dx,&dy, &length);
		//TRACE(L"\length:%d\n", (int)length);
		ang = int(atan2(dy,dx) * 180 / PI + 360) % 360;

		totalDis += length;//累加长度

		//保证字朝上
		if(ang < dPI1 || ang > dPI2)
		{
			bFollowLine = TRUE ;
		}
		else
		{
			bFollowLine = FALSE;
		}

		//是否连续
		if(bFollowLine == bOldFollowLine)
		{
			bContinue = TRUE;
		}
		else
		{
			bContinue = FALSE;
		}

		// 		//太短线处理

		LabelLineInfo.bFollowLine = bFollowLine;
		LabelLineInfo.iCanLabelNum = 0;
		LabelLineInfo.bContinuePre = bContinue;
		LabelLineInfo.fDx = dx;
		LabelLineInfo.fDy = dy;
		LabelLineInfo.fToPreDis = totalDis;//和前面距离累加
		LabelLineInfo.flength = length;
		LabelLineInfo.iIndex = i;
		LabelLineInfo.fStartlen = 0;
		LabelLineInfo.dAng = ang;

		lastDis = length;//开始一个注记的可用长度
		iNoteNum = 0;

		BackupMatrix(TRUE);
		while (lastDis > 0)
		{
			//TRACE(L"\nLastDis:%d\n", (int)lastDis);
			t1 = LabelLineInfo.fStartlen + iNoteNum * fontwidth;
			pt.x = pstLinkPart.m_pLinkPtr[i].x + t1 * dx;
			pt.y = pstLinkPart.m_pLinkPtr[i].y + t1 * dy;
			CreateRect(pt,r, iFontHeight);
			short val = CanLabel(r);			

			if(val > BAN_VALUE)
			{
				if(LabelLineInfo.iCanLabelNum >= 0 )
				{
					LabelLineInfo.fEndLen = t1 + fontwidth/2;
					stLabelLineList.push_back(LabelLineInfo);
				}

				bContinue = FALSE;

				//开始一个新的搜寻
				LabelLineInfo.bFollowLine = bFollowLine;
				LabelLineInfo.iCanLabelNum =0;
				LabelLineInfo.bContinuePre = bContinue;
				LabelLineInfo.fDx = dx;
				LabelLineInfo.fDy = dy;
				LabelLineInfo.fToPreDis = totalDis;
				LabelLineInfo.flength = length;
				LabelLineInfo.iIndex = i;
				LabelLineInfo.fStartlen = t1 + fontwidth/2;
				LabelLineInfo.dAng = ang;
				iNoteNum = 0;				
			}
			else
			{
				LabelLineInfo.iCanLabelNum++;
				bContinue = TRUE;
				iNoteNum ++;
				AddBan(r, BAN_LINENAME_JUDGE_VALUE);
			}
			lastDis -= fontwidth;
		}//while
		BackupMatrix(FALSE);
		if(LabelLineInfo.fStartlen < LabelLineInfo.flength )
		{
			LabelLineInfo.fEndLen = length;
			stLabelLineList.push_back(LabelLineInfo);
		}

		bOldFollowLine = bFollowLine;
	}

	if(totalDis < fontwidth * iTextLen || stLabelLineList.size() == 0)
	{
#ifdef _DEBUG
		TRACE(L"totalDis < stringwide || stLabelLineList.size() == 0");
		TRACE("\n");
#endif
		stLabelLineList.clear();
		if(pstLinkPart.m_pLinkPtr != NULL)
		{
			delete [](pstLinkPart.m_pLinkPtr);
			pstLinkPart.m_pLinkPtr = NULL;
		}
		pstLinkPart.m_iPointNum = 0;
		return;	
	}

	//求最佳注记位置
	//TRACE(L"\n求最佳注记位\n");
	double offset = totalDis / 2.0;
	vector<LineStatInfo> stLineStatInfoList;
	LineStatInfo stLineStatInfo;
	stLabelLine pstLabelLine;
	short int iStart=0;
	short int iEnd=-1;
	short int iCanLabelNum = 0;
	float fStartDis = 0;
	float fEnddis = 0;
	float rat = 0;
	int iLabelNums = 0;

	pstLabelLine = stLabelLineList.at(0);
	fStartDis = pstLabelLine.fToPreDis - (pstLabelLine.flength - pstLabelLine.fStartlen);
	ang = 0;
	double ang1 = 0;

	int nn=0;
	BOOL bfl = TRUE;
	for (i=0; i<stLabelLineList.size(); i++)
	{
		pstLabelLine = stLabelLineList.at(i);
		iLabelNums += pstLabelLine.iCanLabelNum;
		ang += pstLabelLine.dAng * (pstLabelLine.fEndLen - pstLabelLine.fStartlen);
		ang1 += pstLabelLine.dAng * (pstLabelLine.fEndLen - pstLabelLine.fStartlen);

		if(pstLabelLine.bContinuePre == FALSE)
		{
			ang -= pstLabelLine.dAng * (pstLabelLine.fEndLen - pstLabelLine.fStartlen);
			if(iStart <= iEnd/* && fEnddis-fStartDis > stringwide*/)
			{
				stLineStatInfo.dLength = fEnddis - fStartDis;
				stLineStatInfo.dAng = int( ang / (stLineStatInfo.dLength)+ 360) % 360;
				stLineStatInfo.iStartIndex = iStart;
				stLineStatInfo.iEndIndex = iEnd;
				stLineStatInfo.iLabelNums = iLabelNums;
				stLineStatInfo.iPos = nn;
				stLineStatInfo.bFollowLine = bfl;
				stLineStatInfoList.push_back(stLineStatInfo);
				nn++;
			}
			//下一循环
			ang = pstLabelLine.dAng * (pstLabelLine.fEndLen - pstLabelLine.fStartlen);
			fStartDis = pstLabelLine.fToPreDis - (pstLabelLine.flength - pstLabelLine.fStartlen);
			iStart = i;
			iLabelNums = 0;
		}

		iEnd = i;
		fEnddis = pstLabelLine.fToPreDis - (pstLabelLine.flength - pstLabelLine.fEndLen);
		bfl = pstLabelLine.bFollowLine;
	}

	if(iStart <= iEnd /* && fEnddis-fStartDis > stringwide*/)
	{
		stLineStatInfo.iStartIndex = iStart;
		stLineStatInfo.iEndIndex = iEnd;
		stLineStatInfo.iLabelNums = iLabelNums;
		stLineStatInfo.dLength = fEnddis - fStartDis;
		stLineStatInfo.dAng = int(ang / (stLineStatInfo.dLength)+ 360) % 360;
		stLineStatInfo.iPos = nn;
		stLineStatInfo.bFollowLine = stLineStatInfo.bFollowLine;
		stLineStatInfoList.push_back(stLineStatInfo);
		stLineStatInfo.bFollowLine = bfl;
		nn++;
	}
	//排序，标注
	//TRACE(L"\n排序，标注\n");
	stLabelLine lli;
	stLabelLine lli1;
	int iNewStart, iNewEnd;
	if (stLineStatInfoList.size() > 0)
	{
		vector<LineStatInfo> List;
		List.clear();
		for (int ii=0;ii<stLineStatInfoList.size();ii++)
		{
			List.push_back(stLineStatInfoList[ii]);
		}
		double dis;
		qsort(&(stLineStatInfoList[0]), stLineStatInfoList.size(), sizeof(LineStatInfo), compareEx);
		stLineStatInfo = stLineStatInfoList[0];
		dis = stLineStatInfo.dLength;
		iStart = stLineStatInfo.iStartIndex;
		iNewStart = iStart;
		iEnd = stLineStatInfo.iEndIndex;
		iNewEnd = iEnd;
#if 0
		double angle1,angle2,d1,d2;
		int inc1,inc2;

		angle1 = 0;
		angle2 = 0;
		inc1 = 0;
		inc2 = 0;
		d1 = 0;
		d2 = 0;
		stLabelLine lli2;
		for (int j=iStart; j<iEnd+1; j++)
		{
			lli2 = stLabelLineList.at(j);
			dx1 = pstLinkPart.m_pLinkPtr[lli2.iIndex+1].x - pstLinkPart.m_pLinkPtr[lli2.iIndex].x;
			dy1 = pstLinkPart.m_pLinkPtr[lli2.iIndex+1].y - pstLinkPart.m_pLinkPtr[lli2.iIndex].y;
			ang0 = atan2(dy1,dx1) * 180 / PI ;
			if(ang0 > 0)
			{
				d1 += lli2.fEndLen-lli2.fStartlen;
				angle1 += ang0*(lli2.fEndLen-lli2.fStartlen);
				inc1++;
			}
			else
			{
				d2 += lli2.fEndLen-lli2.fStartlen;
				angle2 += ang0*(lli2.fEndLen-lli2.fStartlen);
				inc2++;
			}
		}

		if(inc2 > 0 && inc1 > 0)
		{
			ang = angle1/d1;
			ang1 = angle2/d2;
			if(ang1 > -90)
			{
				ang = (ang + ang1)/2;
			}
			else
			{
				ang = (ang + ang1+360)/2;
			}
		}
		else if(inc2 > 0 && inc1 == 0)
		{
			ang = angle2/d2;
		}
		else if(inc1 > 0 && inc2 == 0)
		{
			ang = angle1/d1;
		}
		ang1 = int(ang+360) %360;
		TRACE(" angle=%d\n", (int)ang1);

		if(ang1 < dPI1 || ang1 > dPI2)
		{
			bFollowLine = TRUE ;
		}
		else
		{
			bFollowLine = FALSE;
		}
#endif
#define M_DIS 0.4
		int nPos = stLineStatInfo.iPos;
		int ii;
		for (ii=nPos-1; ii>-1; ii--)
		{
			LineStatInfo lsf = List[ii];
			if(lsf.bFollowLine != stLineStatInfo.bFollowLine)
			{
				if(lsf.dLength < stLineStatInfo.dLength*M_DIS)
				{
					iNewStart = lsf.iStartIndex;
				}
				else
				{
					break;
				}
			}
			else
			{
				iNewStart = lsf.iStartIndex;
			}
		}

		for(ii=nPos+1; ii<stLineStatInfoList.size(); ii++)
		{
			LineStatInfo lsf = List[ii];
			if(lsf.bFollowLine != stLineStatInfo.bFollowLine)
			{
				if(lsf.dLength < stLineStatInfo.dLength*M_DIS)
				{
					iNewEnd = lsf.iEndIndex;
				}
				else
				{
					break;
				}
			}
			else
			{
				iNewEnd = lsf.iEndIndex;
			}
		}

		lli = stLabelLineList.at(iNewStart);
		lli1 = stLabelLineList.at(iNewEnd);
		dis = lli1.fToPreDis - (lli.fToPreDis - lli.flength + lli.fStartlen);
		if(dis >= stringwide)
		{

		}
		else
		{
			stLabelLineList.clear();
			if(pstLinkPart.m_pLinkPtr != NULL)
			{
				delete [](pstLinkPart.m_pLinkPtr);
				pstLinkPart.m_pLinkPtr = NULL;
			}
			pstLinkPart.m_iPointNum = 0;
			return;	
		}
		TRACE("\n----------");
	}
	else
	{
		stLabelLineList.clear();
		if(pstLinkPart.m_pLinkPtr != NULL)
		{
			delete [](pstLinkPart.m_pLinkPtr);
			pstLinkPart.m_pLinkPtr = NULL;
		}
		pstLinkPart.m_iPointNum = 0;
		return;	
	}
#if 1
	double angle1,angle2,d1,d2;
	int inc1,inc2;

	angle1 = 0;
	angle2 = 0;
	inc1 = 0;
	inc2 = 0;
	d1 = 0;
	d2 = 0;
	stLabelLine lli2;
	for (int j=iNewStart; j<iNewEnd+1; j++)
	{
		lli2 = stLabelLineList.at(j);
		dx1 = pstLinkPart.m_pLinkPtr[lli2.iIndex+1].x - pstLinkPart.m_pLinkPtr[lli2.iIndex].x;
		dy1 = pstLinkPart.m_pLinkPtr[lli2.iIndex+1].y - pstLinkPart.m_pLinkPtr[lli2.iIndex].y;
		ang0 = atan2(dy1,dx1) * 180 / PI ;
		if(ang0 > 0)
		{
			d1 += lli2.fEndLen-lli2.fStartlen;
			angle1 += ang0*(lli2.fEndLen-lli2.fStartlen);
			inc1++;
		}
		else
		{
			d2 += lli2.fEndLen-lli2.fStartlen;
			angle2 += ang0*(lli2.fEndLen-lli2.fStartlen);
			inc2++;
		}
	}

	if(inc2 > 0 && inc1 > 0)
	{
		ang = angle1/d1;
		ang1 = angle2/d2;
		if(ang1 > -90)
		{
			ang = (ang + ang1)/2;
		}
		else
		{
			ang = (ang + ang1+360)/2;
		}
	}
	else if(inc2 > 0 && inc1 == 0)
	{
		ang = angle2/d2;
	}
	else if(inc1 > 0 && inc2 == 0)
	{
		ang = angle1/d1;
	}
	ang1 = int(ang+360) %360;
	TRACE(" angle=%d\n", (int)ang1);

	if(ang1 < dPI1 || ang1 > dPI2)
	{
		bFollowLine = TRUE ;
	}
	else
	{
		bFollowLine = FALSE;
	}
#endif

	//可注记长度
	lli = stLabelLineList.at(iNewStart);
	lli1 = stLabelLineList.at(iNewEnd);
	t1 = lli1.fToPreDis - lli1.flength + lli1.fEndLen - lli.fToPreDis + lli.flength - lli.fStartlen;
	if(t1 < fontwidth * iTextLen)
	{
		stLineStatInfoList.clear();
		stLabelLineList.clear();
		if(pstLinkPart.m_pLinkPtr != NULL)
		{
			delete [](pstLinkPart.m_pLinkPtr);
			pstLinkPart.m_pLinkPtr = NULL;
		}
		pstLinkPart.m_iPointNum = 0;
#ifdef _DEBUG
		TRACE(L"t1 < fontwidth * iTextLen");
		TRACE("\n");
#endif
		return;
	}

	//偏移和间距
	offset = (t1 - (fSpace + fontwidth * iTextLen)) / 2.0;
	if(offset < fontwidth)
	{
		fSpace = (t1 - fontwidth * iTextLen) / iTextLen;
		if(fSpace < fontwidth/2)
		{
			fSpace = 0;
		}
        // !!! 20110402 liujingzhou modify 直接设置为0，为了更多的标注道路
        fSpace = 0;
	}

	dis = 0;
	float offdis = offset;
	int kw = 0;//
	COLORREF clr =  GetTextColor(hDC); 
	//////////////////////////////////////////////////////////////////////////
	
	RECT* pPosDraw = new RECT[iTextLen];
	if(pPosDraw)
	{
		BOOL labelFinish = FALSE;
		BackupMatrix(TRUE);
		//////////////////////////////////////////////////////////////////////////
		for (i=/*iNewStart*/0; i<=/*iNewEnd*/stLabelLineList.size()-1;)
		{
			if(kw >= iTextLen)
			{
				//注记完成
				labelFinish = TRUE;
				break;
			}

			lli = stLabelLineList[i];
			t1 = lli.fEndLen - lli.fStartlen;
			//取下一条线注记
			if(offdis > dis + t1)
			{
				dis += t1;
				i++;
				continue;
			}

			pt.x = pstLinkPart.m_pLinkPtr[lli.iIndex].x + (lli.fStartlen + offdis - dis) * lli.fDx;
			pt.y = pstLinkPart.m_pLinkPtr[lli.iIndex].y + (lli.fStartlen + offdis - dis) * lli.fDy;
			CreateRect(pt,r, iFontHeight);

			BOOL T = IsCanLabel(r);
			if(!T)
			{
				offdis += fSpace;
				pt.x = pstLinkPart.m_pLinkPtr[lli.iIndex].x + (lli.fStartlen + offdis - dis) * lli.fDx;
				pt.y = pstLinkPart.m_pLinkPtr[lli.iIndex].y + (lli.fStartlen + offdis - dis) * lli.fDy;
				CreateRect(pt,r, iFontHeight);
				T = IsCanLabel(r);
			}
			if(T)
			{
				/*
				if(bFollowLine == TRUE)
				{
				if( !((pTextStyle->m_drawtype&eTEXT_DT_OUTLINEFONT) > 0) )
				{
				SetTextColor(hDC, pTextStyle->m_clrFont);
				::DrawText(hDC, &szName[kw], 1, &r, DT_WORDBREAK);
				}
				else
				{
				pCanvas->DrawEdgeText(&szName[kw], 1, &r, pTextStyle->m_clrFont, pTextStyle->m_clrBack);
				}
				}
				else
				{
				if( !((pTextStyle->m_drawtype&eTEXT_DT_OUTLINEFONT) > 0) )
				{
				SetTextColor(hDC, pTextStyle->m_clrFont);
				::DrawText(hDC, &szName[iTextLen-kw-1], 1, &r, DT_WORDBREAK);
				}
				else
				{
				pCanvas->DrawEdgeText(&szName[iTextLen-kw-1], 1, &r, pTextStyle->m_clrFont, pTextStyle->m_clrBack);
				}
				}
				*/
				pPosDraw[kw] = r;
				AddBan(r, BAN_LINENAME_JUDGE_VALUE);
				kw ++;
				offdis += (fontwidth + fSpace);
			}
			else
			{						
				kw = 0;//
				offdis += (fontwidth + fSpace);
			}		
		}	
		//////////////////////////////////////////////////////////////////////////
		if(labelFinish)
		{
			double dx,dy;

			dx = pPosDraw[1].left - pPosDraw[0].left;
			dy = pPosDraw[1].top - pPosDraw[0].top;
			int ang = int(atan2(dy,dx) * 180 / PI + 360) % 360;

			if(ang <= dPI1 || ang >= dPI2)
			{
				bFollowLine = TRUE;
			}
			else
			{
				bFollowLine = FALSE;
			}

			TRACE( "DYWANG  Angle = %d\n", ang);

			if(bFollowLine == TRUE)
			{

				if( !((pTextStyle->m_drawtype&eTEXT_DT_OUTLINEFONT) > 0) )
				{
					for(int kw = 0; kw<iTextLen; kw++)
					{
						::DrawText(hDC, &szName[kw], 1, pPosDraw+kw, DT_WORDBREAK);
					}
				}
				else
				{
					for(int kw = 0; kw<iTextLen; kw++)
					{
						pCanvas->DrawEdgeText(&szName[kw], 1, pPosDraw+kw, pTextStyle->m_clrFont, pTextStyle->m_clrBack);
					}
				}


			}
			else
			{
				if( !((pTextStyle->m_drawtype&eTEXT_DT_OUTLINEFONT) > 0) )
				{
					for(int kw = 0; kw<iTextLen; kw++)
					{
						::DrawText(hDC, &szName[iTextLen-kw-1], 1, pPosDraw+kw, DT_WORDBREAK);
					}
				}
				else
				{
					for(int kw = 0; kw<iTextLen; kw++)
					{
						pCanvas->DrawEdgeText(&szName[iTextLen-kw-1], 1, pPosDraw+kw, pTextStyle->m_clrFont, pTextStyle->m_clrBack);
					}
				}
			}
		}
		else
		{
			BackupMatrix(FALSE);
		}
		delete[] pPosDraw;
		pPosDraw = NULL;
	}
	//////////////////////////////////////////////////////////////////////////
	SetTextColor(hDC,clr);
	TRACE("注记完成");
	TRACE("\n");

	//TRACE(L"\nEND注记\n");
	//释放空间
	stLineStatInfoList.clear();
	stLabelLineList.clear();
	if(pstLinkPart.m_pLinkPtr != NULL)
	{
		delete [](pstLinkPart.m_pLinkPtr);
		pstLinkPart.m_pLinkPtr = NULL;
	}
	pstLinkPart.m_iPointNum = 0;
}




