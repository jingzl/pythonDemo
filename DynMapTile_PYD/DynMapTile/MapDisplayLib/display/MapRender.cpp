#include "stdafx.h"
#include "MapRender.h"

CMapRender::CMapRender(void)
{

}

CMapRender::~CMapRender(void)
{

}

BOOL CMapRender::DrawMap(CDisplayInfo* pDisplayInfo, DWORD dwBackColor, PMAP_DISP_DATA pData, HDC* pDC, 
                         PTILETEXT_INFO pTileTextInfo, BOOL bGridLine)
{
	if(pDisplayInfo && pData && pDC)
	{
#ifdef MAPDISPLAY_LOG
        TCHAR szMsg[MAX_PATH] = {0};
        int nTickCount = GetTickCount();
#endif

        // �������屳����ɫ
        DrawBack(pDisplayInfo, dwBackColor);

#ifdef MAPDISPLAY_LOG
        nTickCount = GetTickCount() - nTickCount;
        _stprintf( szMsg, _T("DrawBack--%d\n"), nTickCount );
        TRACE( szMsg );

        nTickCount = GetTickCount();
#endif

        // ���Ʊ�������
        pDisplayInfo->GetLabelAdjust()->BeginDraw();
		m_backRender.Draw(pDisplayInfo, pData);

#ifdef MAPDISPLAY_LOG
        nTickCount = GetTickCount() - nTickCount;
        _stprintf( szMsg, _T("backRender--%d\n"), nTickCount );
        TRACE( szMsg );

        nTickCount = GetTickCount();
#endif
        // ���Ƶ�·����
		m_roadRender.Draw(pDisplayInfo, pData);
#ifdef MAPDISPLAY_LOG
        nTickCount = GetTickCount() - nTickCount;
        _stprintf( szMsg, _T("roadRender--%d\n"), nTickCount );
        TRACE( szMsg );

        nTickCount = GetTickCount();
#endif

        // ������ʾ��
        // ����� > ��·�� > ע�� > POI

        // �������������
        m_SpecialRender.Draw( pDisplayInfo, pData, pTileTextInfo );

#ifdef MAPDISPLAY_LOG
        nTickCount = GetTickCount() - nTickCount;
        _stprintf( szMsg, _T("special point render--%d\n"), nTickCount );
        TRACE( szMsg );

        nTickCount = GetTickCount();
#endif
        // ���Ƶ�·��������
        m_roadRender.DrawName(pDisplayInfo, pData);

#ifdef MAPDISPLAY_LOG
        nTickCount = GetTickCount() - nTickCount;
        _stprintf( szMsg, _T("roadName render -- %d\n"), nTickCount );
        TRACE( szMsg );

        nTickCount = GetTickCount();
#endif
        // ����ע������
		m_annoRender.Draw(pDisplayInfo, pData, pTileTextInfo);

#ifdef MAPDISPLAY_LOG
        nTickCount = GetTickCount() - nTickCount;
        _stprintf( szMsg, _T("annoRender--%d\n"), nTickCount );
        TRACE( szMsg );

        nTickCount = GetTickCount();
#endif
        // MCMDN����ת��������û��POI����
        // ����POI����
		//m_poiRender.Draw(pDisplayInfo, pData, pTileTextInfo);

#ifdef MAPDISPLAY_LOG
        nTickCount = GetTickCount() - nTickCount;
        _stprintf( szMsg, _T("poiRender--%d\n"), nTickCount );
        TRACE( szMsg );
#endif

        if ( bGridLine )
        {
            // ���Ƹ�������
            m_backRender.DrawGridRect( pDisplayInfo, &pData->gridrectLayer );
        }

        pDisplayInfo->GetLabelAdjust()->EndDraw();

		*pDC = pDisplayInfo->GetHDC();

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CMapRender::DrawBack(CDisplayInfo* pDisplayInfo, DWORD dwBackColor)
{
	if(pDisplayInfo)
	{
		RECT rcRect = pDisplayInfo->GetBufRect();
		CCanvas* pCanvas = pDisplayInfo->GetCanvas();
		if(pCanvas)
		{
			GDIPen pen;
			pen.color.a = 255;
			pen.color.r = GetRValue(dwBackColor);
			pen.color.g = GetGValue(dwBackColor);
			pen.color.b = GetBValue(dwBackColor);
			pen.width = 1;
			pen.flags = 0;
			pCanvas->DrawBack(dwBackColor, &rcRect);
		}
	}
}