#include "stdafx.h"
#include "IMapDisplay.h"
#include "IParser.h"
#include "IMapOutput.h"
#include "MapOutput.h"
#include "Quantize.h"



CMapOutput* CMapOutput::m_sinstance = NULL;
int CMapOutput::m_nError = 0;

CMapOutput::CMapOutput():m_Cximage(CXIMAGE_FORMAT_BMP)
{
    m_lpLogFunc = NULL;
    m_nBigDisplayMode = 0;
    m_pParser = NULL;
    m_pMapDisplay = NULL;
    ZeroMemory( m_szOutputMapPath, sizeof(TCHAR)*MAX_PATH );
    ZeroMemory( m_szOutputTxtPath, sizeof(TCHAR)*MAX_PATH );

    m_hDC = NULL;
    m_hMemDC = NULL;
    m_hBitmap = NULL;
    m_hOldBitmap = NULL;
    ZeroMemory( &m_Rect, sizeof(RECT) );
    ZeroMemory( &m_rcGeoRect, sizeof(GEORECT) );
    m_rcAllMapGeoRect.dMinX = MAPRECT_MINX;
    m_rcAllMapGeoRect.dMinY = MAPRECT_MINY;
    m_rcAllMapGeoRect.dMaxX = MAPRECT_MAXX;
    m_rcAllMapGeoRect.dMaxY = MAPRECT_MAXY;

    m_lpMapDispData = NULL;
    m_nPicMapWidth = 256;
    m_nPicPoiWidth = 256;

    ZeroMemory( m_szFilename, sizeof(TCHAR)*MAX_PATH );
    ZeroMemory( m_szDirectroy, sizeof(TCHAR)*MAX_PATH );
}

CMapOutput* CMapOutput::Instance()
{
    if ( m_sinstance == NULL )
    {
        static CMapOutput mapoutput;
        m_sinstance = &mapoutput;
    }
    return m_sinstance;
}

CMapOutput::~CMapOutput()
{
    Release();
}

void CMapOutput::SetLogFunc( LogFunc lplogfunc )
{
    ASSERT( lplogfunc != NULL );
	m_lpLogFunc = lplogfunc;
}

BOOL CMapOutput::InitMapOutput(IMapDisplay* pMapDisplay, IParser* pParser, const char* lpszPicPath, const char* lpszTxtPath )
{
    if ( pMapDisplay == NULL || pParser == NULL || strlen(lpszPicPath) <=0 || strlen(lpszTxtPath) <= 0 )
    {
        return FALSE;
    }
    m_pMapDisplay = pMapDisplay;
    m_pParser = pParser;

    wstring wstr = str2wstr( lpszPicPath );
    _tcscpy( m_szOutputMapPath, wstr.c_str() );
    wstr = str2wstr( lpszTxtPath );
    _tcscpy( m_szOutputTxtPath, wstr.c_str() );
    OutputMessage( m_szOutputMapPath );
    OutputMessage( m_szOutputTxtPath );

    m_nPicMapWidth = 256;
    m_nPicPoiWidth = 256;

    m_Cximage.Create( m_nPicMapWidth, m_nPicMapWidth, 8, CXIMAGE_FORMAT_BMP );

    return TRUE;
}

BOOL CMapOutput::OutputMap( int iZoomx, double Xmin, double Ymin, double Xmax, double Ymax, BYTE byteBigDisplayMode )
{
    m_nBigDisplayMode = byteBigDisplayMode;

    CString strMsg;
    if ( !CutMapByRect( iZoomx, Xmin, Ymin, Xmax, Ymax ) )
    {
        strMsg.Format( _T("CutMapByRect - %d - (%.6f,%.6f;%.6f,%.6f) failed !"), 
            iZoomx, Xmin, Ymin, Xmax, Ymax );
        OutputMessage( strMsg );
        return FALSE;
    }
    strMsg.Format( L"OutPut MapGrid Zoom %d Rect is Finished !", iZoomx );
    OutputMessage( strMsg );

    return TRUE;
}

void CMapOutput::Release()
{
    m_hDC = NULL;
    if ( m_hMemDC != NULL )
    {
        DeleteDC( m_hMemDC );
        m_hMemDC = NULL;
    }
    if ( m_hBitmap != NULL )
    {
        DeleteObject( m_hBitmap );
        m_hBitmap = NULL;
    }
    m_hOldBitmap = NULL;

    m_Cximage.Destroy();
}

void CMapOutput::OutputMessage( CString str )
{
	if ( m_lpLogFunc != NULL )
	{
        m_lpLogFunc( str );
	}
    else
    {
        _tprintf( str );
        _tprintf( _T("\n") );
    }
}

//////////////////////////////////////////////////////////////////////////
// 
BOOL CMapOutput::CutMapByRect(int iZoomx, double Xmin, double Ymin, double Xmax, double Ymax)
{
    CString strMsg;
    if ( iZoomx == 0 )
    {
        // 0��������ֻ��һ��ͼƬ�������ڻ���HDC��BUFFER��512��������Ҫ�������⴦��
        GEOPOINT pos(MAP_MIN_X, MAP_MAX_Y);
        if ( !DrawCutMap( iZoomx, pos, NULL ) )
        {
            return FALSE;
        }
        RECT rect = {0, 0, m_nPicMapWidth, m_nPicMapWidth};
        rect.left = 0;
        rect.right = m_nPicMapWidth;
        rect.bottom = m_nPicMapWidth;
        rect.top = 0;
        CopyDCToBitmap( &rect );
        if ( m_hBitmap == NULL )
        {
            strMsg.Format( _T("CopyDCToBitmap - NULL") );
            OutputMessage( strMsg );
            return FALSE;
        }
        if ( !CreateXimages( iZoomx, 0, 0, CXIMAGE_FORMAT_PNG ) )
        {
            strMsg.Format( _T("CreateXimages failed !") );
            OutputMessage( strMsg );
            return FALSE;
        }
    }
    else
    {
        // ��������
        int pixcelX1, pixcelX2, pixcelY1, pixcelY2;
        // ���Ͻ�
        m_projectTrans.LongLatToPixelXY( Xmin, Ymax, iZoomx, pixcelX1, pixcelY1 );
        // ���½�
        m_projectTrans.LongLatToPixelXY( Xmax, Ymin, iZoomx, pixcelX2, pixcelY2 );

        // ����ֵ
        int minCol, maxCol, minRow, maxRow;
        bool bx1 = false;
        bool by1 = false;
        bool bx2 = false;
        bool by2 = false;
        m_projectTrans.pixelXYToTileXY( pixcelX1, pixcelY1, minCol, minRow, bx1, by1 );
        m_projectTrans.pixelXYToTileXY( pixcelX2, pixcelY2, maxCol, maxRow, bx2, by2 );
        // ֻ������½ǽ��д���
        if ( bx2 )
        {
            maxCol -= 1;
        }
        if ( by2 )
        {
            maxRow -= 1;
        }
        if ( !CutMapByRC( iZoomx, minRow, minCol, maxRow, maxCol ) )
        {
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CMapOutput::CutMapByRC( int iZoomx, int mini, int minj, int maxi, int maxj )
{
    CString strMsg;
    long lNums = HDC_SIZE_WEIGHT/m_nPicMapWidth;
    RECT rect = {0, 0, m_nPicMapWidth, m_nPicMapWidth};
    GEOPOINT pos;
    long lRow = 0;
    long lCol = 0;

    // ����Buf���õ�512���е���ͼ������Ҫ/2����һ��
    int nBufMinRow = mini / 2;
    int nBufMinCol = minj / 2;
    int nBufMaxRow = maxi / 2;
    int nBufMaxCol = maxj / 2;

    int x, y;
    m_projectTrans.LongLatToPixelXY(MAP_MIN_X,MAP_MAX_Y, iZoomx, x, y);

    // ��
    for ( int ix=nBufMinRow; ix<=nBufMaxRow; ix++ )
    {
        // ��
        for ( int iy=nBufMinCol; iy<=nBufMaxCol; iy++ )
        {
            m_projectTrans.PixelXYToLongLat(x +iy*HDC_SIZE_WEIGHT, y+ix*HDC_SIZE_WEIGHT, iZoomx, pos.x, pos.y);

            ST_TILETEXT_INFO tileTextInfo;
            if ( !DrawCutMap( iZoomx, pos, &tileTextInfo ) )
            {
                return FALSE;
            }
            for (int i = 0; i < lNums; i++)
            {
                for (int j = 0; j < lNums; j++)
                {
                    rect.left = 0 + j * m_nPicMapWidth;
                    rect.right = m_nPicMapWidth + j * m_nPicMapWidth;
                    rect.bottom = m_nPicMapWidth + i * m_nPicMapWidth;
                    rect.top = 0 + i * m_nPicMapWidth;
                    CopyDCToBitmap( &rect );
                    if ( m_hBitmap == NULL )
                    {
                        strMsg.Format( _T("CopyDCToBitmap - NULL") );
                        OutputMessage( strMsg );
                        return FALSE;
                    }
                    lRow = ix * lNums + i;
                    lCol = iy * lNums + j;

                    if ( !CreateXimages( iZoomx, lRow, lCol, CXIMAGE_FORMAT_PNG ) )
                    {
                        strMsg.Format( _T("CreateXimages failed !") );
                        OutputMessage( strMsg );
                        return FALSE;
                    }

                    GEORECT rc;
                    m_projectTrans.tileXYToLLBound( lCol, lRow, iZoomx, rc.dMinX, rc.dMaxY, rc.dMaxX, rc.dMinY );
                    if ( !CreateTileTextFile( iZoomx, lRow, lCol, rc, &tileTextInfo ) )
                    {
                        strMsg.Format( _T("CreateTileTextFile failed !") );
                        OutputMessage( strMsg );
                        return FALSE;
                    }
                }
            }
        }
    }

    return TRUE;
}

BOOL CMapOutput::DrawCutMap( int iZoomx, GEOPOINT pos, PTILETEXT_INFO pTileTextInfo )
{
    CString strMsg;
	if ( !m_pMapDisplay->SetMapScale( 0, iZoomx ) )
	{
		strMsg.Format( _T("SetMapScale %d Fail !!!"), iZoomx );
		OutputMessage( strMsg );
		return FALSE;
	}

    // �����Ͻǿ�ʼ����
	if ( !m_pMapDisplay->SetMapPos( pos, MP_LEFT_TOP))
	{
		strMsg.Format( _T("SetMapPos %.6f-%.6f Fail !!!"), pos.x, pos.y );
		OutputMessage( strMsg );
		return FALSE;
	}

	m_rcGeoRect = m_pMapDisplay->GetMapRect();
	if ( !m_rcAllMapGeoRect.IsIntersected( &m_rcGeoRect ) )
	{
        // ��ǰ��Ҫ���Ƶķ�Χ�뵱ǰ�������ݵķ�Χ���ཻ����ֱ�ӷ���
        strMsg.Format( _T("Out of the Map Bound !!!") );
        OutputMessage( strMsg );
        return FALSE;
	}
	else
	{
		if ( m_rcGeoRect.dMinX < m_rcAllMapGeoRect.dMinX )
		{
			m_rcGeoRect.dMinX = m_rcAllMapGeoRect.dMinX;
		}
		if ( m_rcGeoRect.dMinY < m_rcAllMapGeoRect.dMinY )
		{
			m_rcGeoRect.dMinY = m_rcAllMapGeoRect.dMinY;
		}
		if ( m_rcGeoRect.dMaxX > m_rcAllMapGeoRect.dMaxX )
		{
			m_rcGeoRect.dMaxX = m_rcAllMapGeoRect.dMaxX;
		}
		if ( m_rcGeoRect.dMaxY > m_rcAllMapGeoRect.dMaxY )
		{
			m_rcGeoRect.dMaxY = m_rcAllMapGeoRect.dMaxY;
		}
	}
    m_lpMapDispData = NULL;
	if ( m_pParser->GetMapData( m_nBigDisplayMode, iZoomx, m_rcGeoRect, m_lpMapDispData ) )
	{
        // 2011.9.29. �޸ĵ�����
        // ԭ������ʽ�����������ݣ����ֻ���ں������״���ݣ����ж�Ϊ�գ�������������
        // �޸Ĵ���ʽ������Ŀǰ�����еĺܶൺ�춼��ͨ��������Χס�����γɣ������������µ��춪ʧ�����������ܻ�������ȫ������ռ�ÿռ�Ӵ�
        // 
        /*if ( (byteOnlySealine==1) && m_lpMapDispData->roadLayers.empty() && m_lpMapDispData->annoLayers.empty() && m_lpMapDispData->poiLayers.empty() )
        {
            bOnlySealine = TRUE;
        }*/

        if ( !m_pMapDisplay->DrawMap( m_lpMapDispData->clrback, m_lpMapDispData, &m_hDC, pTileTextInfo ) )
        {
            m_pParser->ReleaseMapData( m_lpMapDispData );
            strMsg.Format( _T("DrawCutMap - DrawMap failed !!!") );
            OutputMessage( strMsg );
            strMsg.Format( _T("SCALE-[%d] ** RECT-(%.6f, %.6f, %.6f, %.6f)"), iZoomx, 
                m_rcGeoRect.dMinX, m_rcGeoRect.dMinY, m_rcGeoRect.dMaxX, m_rcGeoRect.dMaxY );
            OutputMessage( strMsg );
            return FALSE;
        }
	}
	else
	{
        m_pParser->ReleaseMapData( m_lpMapDispData );
		strMsg.Format( _T("DrawCutMap - GetMapData failed !!!") );
		OutputMessage( strMsg );
        strMsg.Format( _T("SCALE-[%d] ** RECT-(%.6f, %.6f, %.6f, %.6f)"), iZoomx, 
            m_rcGeoRect.dMinX, m_rcGeoRect.dMinY, m_rcGeoRect.dMaxX, m_rcGeoRect.dMaxY );
        OutputMessage( strMsg );
        return FALSE;
	}
	m_pParser->ReleaseMapData( m_lpMapDispData );
	return TRUE;
}

void CMapOutput::CopyDCToBitmap( LPRECT lpRect )
{
    if ( m_hDC == NULL || lpRect == NULL || IsRectEmpty(lpRect) )
    {
        return;
    }

    if ( m_hMemDC == NULL )
    {
        //Ϊָ���豸�����������ݵ��ڴ��豸������
        m_hMemDC = CreateCompatibleDC( m_hDC );
        // ����һ����ָ���豸��������ݵ�λͼ
        m_hBitmap = CreateCompatibleBitmap( m_hDC, m_nPicMapWidth, m_nPicMapWidth );
    }
    // ���ѡ����������
    m_Rect = *lpRect;
    // ����λͼѡ���ڴ��豸��������
    m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
    // ����Ļ�豸�����������ڴ��豸��������
    StretchBlt( m_hMemDC, 0, 0, m_nPicMapWidth, m_nPicMapWidth, m_hDC, m_Rect.left, m_Rect.top, m_nPicMapWidth, m_nPicMapWidth, SRCCOPY );
    //TransparentBlt( m_hMemDC, 0, 0, m_nPicMapWidth, m_nPicMapWidth, m_hDC, m_Rect.left, m_Rect.top, m_nPicMapWidth, m_nPicMapWidth, RGB(0x94,0xba,0xde) );


#if 0

    // --debug
    // ����͸��ɫͼƬ
    HBITMAP hOldImageBMP, hImageBMP = CreateCompatibleBitmap(m_hMemDC, m_nPicMapWidth, m_nPicMapWidth);    // ��������λͼ
    HBITMAP hOldMaskBMP, hMaskBMP = CreateBitmap(m_nPicMapWidth, m_nPicMapWidth, 1, 1, NULL);            // ������ɫ����λͼ
    HDC        hImageDC = CreateCompatibleDC(m_hMemDC);
    HDC        hMaskDC = CreateCompatibleDC(m_hMemDC);
    hOldImageBMP = (HBITMAP)SelectObject(hImageDC, hImageBMP);
    hOldMaskBMP = (HBITMAP)SelectObject(hMaskDC, hMaskBMP);

    // ��ԴDC�е�λͼ��������ʱDC��
    StretchBlt(hImageDC, 0, 0, m_nPicMapWidth, m_nPicMapWidth, m_hDC, m_Rect.left, m_Rect.top, m_nPicMapWidth, m_nPicMapWidth, SRCCOPY);

    // ����͸��ɫ
    SetBkColor(hImageDC, RGB(0x94,0xba,0xde) );

    // ����͸������Ϊ��ɫ����������Ϊ��ɫ������λͼ
    BitBlt(hMaskDC, 0, 0, m_nPicMapWidth, m_nPicMapWidth, hImageDC, 0, 0, SRCCOPY);

    // ����͸������Ϊ��ɫ���������򱣳ֲ����λͼ
    SetBkColor(hImageDC, RGB(0,0,0));
    SetTextColor(hImageDC, RGB(255,255,255));
    BitBlt(hImageDC, 0, 0, m_nPicMapWidth, m_nPicMapWidth, hMaskDC, 0, 0, SRCAND);

    // ͸�����ֱ�����Ļ���䣬�������ֱ�ɺ�ɫ
    SetBkColor(m_hMemDC,RGB(0xff,0xff,0xff));
    SetTextColor(m_hMemDC,RGB(0,0,0));
    BitBlt(m_hMemDC, 0, 0, m_nPicMapWidth, m_nPicMapWidth, hMaskDC, 0, 0, SRCAND);

    // "��"����,��������Ч��
    BitBlt(m_hMemDC, 0, 0, m_nPicMapWidth, m_nPicMapWidth, hImageDC, 0, 0, SRCPAINT);

    SelectObject(hImageDC, hOldImageBMP);
    DeleteDC(hImageDC);
    SelectObject(hMaskDC, hOldMaskBMP);
    DeleteDC(hMaskDC);
    DeleteObject(hImageBMP);
    DeleteObject(hMaskBMP);
    // --debug
#endif


    //�õ���Ļλͼ�ľ��
    m_hBitmap = (HBITMAP)SelectObject( m_hMemDC, m_hOldBitmap );
}

BOOL CMapOutput::CreateXimages( int iZoomx, long lRow, long lCol, DWORD imagetype )
{
    CString strMsg;
    if ( !m_Cximage.CreateFromHBITMAP( m_hBitmap ) )
    {
        CString strErr;
        strErr = (char*)m_Cximage.GetLastError();
        strMsg.Format( _T("CreateXimages - CreateFromHBITMAP fail ! -- %s "), strErr );
        OutputMessage( strMsg );
        return FALSE;
    }

    //if ( m_eCreateMode == eCutMapByZoom || m_eCreateMode == eCutMapByRect || m_eCreateMode == eCutMapByRC )
    {
        // ��ͼ���������8λ��PNG
        // 
        // �Ż�����СPNG���ļ�size
        // תΪ8λ���PNGͼƬ
        long colors = 256;
        RGBQUAD* ppal = NULL;
        CQuantizer q(colors,(colors>16?7:8));
        q.ProcessImage(m_Cximage.GetDIB());
        ppal=(RGBQUAD*)calloc(256*sizeof(RGBQUAD),1);
        q.SetColorTable(ppal);
        bool b = m_Cximage.DecreaseBpp( 8, false, ppal, colors );
        if (ppal) free(ppal);
    }
    //else if ( m_eCreateMode == eCutPoiByZoom || m_eCreateMode == eCutPoiByRect || m_eCreateMode == eCutPoiByRC )
    {
        /*
        // POI����Ϊ͸��ͼ��
        // ����Ϊ32λ
        // 
        // ����alpha��
        m_Cximage.AlphaCreate();
        // �������ɨ��
        for (int i=0; i<m_Cximage.GetHeight(); ++i)
        {
            for (int j=0; j<m_Cximage.GetWidth(); ++j)
            {
                RGBQUAD crPix =  m_Cximage.GetPixelColor(j, i);
                // �ж�crPix�ǲ��ǵ��ڱ���ɫ
                if ( crPix.rgbRed == CN_TRANS_COLOR.rgbRed && crPix.rgbGreen == CN_TRANS_COLOR.rgbGreen && crPix.rgbBlue == CN_TRANS_COLOR.rgbBlue )
                {
                    // ͸��
                    m_Cximage.AlphaSet( j, i, 0 );
                }
                else
                {
                    // ��͸��
                    m_Cximage.AlphaSet( j, i, 255 );
                }
            }
        }
        */
    }

    ZeroMemory( m_szFilename, sizeof(TCHAR)*MAX_PATH );
    ZeroMemory( m_szDirectroy, sizeof(TCHAR)*MAX_PATH );

    // !!! ����WebӦ�õļ����㷨�����뱣֤0��ֻ��һ��ͼƬ��������Ȼ��Ҫ���������ߡ�
    // �������˱�֤�����߲��������£�ͨ���������ʱ���ļ��м��ļ�����������Web�˵�����
    // !!!
    GetMapDirectory( iZoomx, lRow, lCol, m_szFilename );
    _stprintf( m_szDirectroy, _T("%s%s"), m_szOutputMapPath, m_szFilename );
    if ( !IsDirectoryExist( m_szDirectroy ) )
    {
        Create_Directory( m_szDirectroy );
    }
    _stprintf( m_szFilename, _T("%s\\%d_%d_%d.png"), m_szDirectroy, iZoomx, lRow, lCol );
    if ( !m_Cximage.Save( m_szFilename, imagetype ) )
    {
        strMsg.Format( _T("CreateXimages - Save Fail ! - %s - %s"), m_szFilename, m_Cximage.GetLastError() );
        OutputMessage( strMsg );
        return FALSE;
    }
    m_Cximage.Destroy();

    return TRUE;
}

BOOL CMapOutput::CreateTileTextFile( int iZoomx, long lRow, long lCol, GEORECT& rect, PTILETEXT_INFO pTileTextInfo )
{
    CString strMsg;
    ZeroMemory( m_szFilename, sizeof(TCHAR)*MAX_PATH );
    ZeroMemory( m_szDirectroy, sizeof(TCHAR)*MAX_PATH );

    // ��ͼƬ�ļ�����һ������֯�ṹ
    // ��ÿ������zoomĿ¼��������� _t ����������
    // �ڲ��ṹ��ȫһ��
    GetMapDirectory( iZoomx, lRow, lCol, m_szFilename, TRUE );
    _stprintf( m_szDirectroy, _T("%s%s"), m_szOutputTxtPath, m_szFilename );
    if ( !IsDirectoryExist( m_szDirectroy ) )
    {
        Create_Directory( m_szDirectroy );
    }
    _stprintf( m_szFilename, _T("%s\\%d_%d_%d.t"), m_szDirectroy, iZoomx, lRow, lCol );
    pTileTextInfo->m_nScale = iZoomx;
    pTileTextInfo->m_nTileX = lCol;
    pTileTextInfo->m_nTileY = lRow;
    if ( !m_TileTextWriter.Save( m_szFilename, rect, pTileTextInfo ) )
    {
        strMsg.Format( _T("TileTextWrite - Save Fail ! - %s"), m_szFilename );
        OutputMessage( strMsg );
        return FALSE;
    }

    return TRUE;
}

BOOL CMapOutput::CreateMapDirectory( LPCTSTR lpPathName, int iZoomx, BOOL bTSuffix )
{
	TCHAR strname[MAX_PATH] = {0};
	_stprintf( strname, _T("%s"), lpPathName );
	CreateDirectory(strname, NULL);

    TCHAR szTSuffix[5] = {0};
    if ( bTSuffix != NULL )
    {
        _tcscpy( szTSuffix, _T("_t") );
    }

    if ( m_nPicMapWidth == 256 )
    {
        _stprintf( strname, _T("%s\\zoom_%d%s"), lpPathName, iZoomx, szTSuffix );
        CreateDirectory( strname, NULL );
        if ( iZoomx <= 4 )
        {
            _stprintf( strname, _T("%s\\zoom_%d%s\\0"), lpPathName, iZoomx, szTSuffix );
            CreateDirectory( strname, NULL );
        }
        else if ( iZoomx <= 8 )
        {
            int iFolders = pow( 4.0, ( iZoomx - 4 ) );
            for ( int i = 0; i < iFolders; i++ )
            {
                _stprintf( strname, _T("%s\\zoom_%d%s\\%d"), lpPathName, iZoomx, szTSuffix, i );
                CreateDirectory( strname, NULL );
            }
        }
        else if ( iZoomx <= 12 )
        {
            int iFolders = pow(4.0, ( iZoomx - 8 ) );
            for ( int i = 0; i < iFolders; i++ )
            {
                _stprintf( strname, _T("%s\\zoom_%d%s\\%d"), lpPathName, iZoomx, szTSuffix, i );
                CreateDirectory( strname, NULL );
                for ( int j = 0; j < FILE_COUNT_MAX; j++ )
                {
                    _stprintf( strname, _T("%s\\zoom_%d%s\\%d\\%d"), lpPathName, iZoomx, szTSuffix, i, j );
                    CreateDirectory( strname, NULL );
                }
            }
        }
        else if ( iZoomx <= 16 )
        {
            int iFolders = pow(4.0, ( iZoomx - 12 ) );
            for ( int i = 0; i < iFolders; i++ )
            {
                _stprintf( strname, _T("%s\\zoom_%d%s\\%d"), lpPathName, iZoomx, szTSuffix, i );
                CreateDirectory( strname, NULL );

                for ( int j = 0; j < FILE_COUNT_MAX; j++ )
                {
                    _stprintf( strname, _T("%s\\zoom_%d%s\\%d\\%d"), lpPathName, iZoomx, szTSuffix, i, j );
                    CreateDirectory( strname, NULL );

                    for ( int k = 0; k < FILE_COUNT_MAX; k++ )
                    {
                        _stprintf( strname, _T("%s\\zoom_%d%s\\%d\\%d\\%d"), lpPathName, iZoomx, szTSuffix, i, j, k );
                        CreateDirectory( strname, NULL );
                    }
                }
            }
        }
        else if ( iZoomx <= 20 )
        {
            int iFolders = pow(4.0, ( iZoomx - 16 ) );
            for ( int i = 0; i < iFolders; i++ )
            {
                _stprintf( strname, _T("%s\\zoom_%d%s\\%d"), lpPathName, iZoomx, szTSuffix, i );
                CreateDirectory( strname, NULL );

                for ( int j = 0; j < FILE_COUNT_MAX; j++ )
                {
                    _stprintf( strname, _T("%s\\zoom_%d%s\\%d\\%d"), lpPathName, iZoomx, szTSuffix, i, j );
                    CreateDirectory( strname, NULL );

                    for ( int k = 0; k < FILE_COUNT_MAX; k++ )
                    {
                        _stprintf( strname, _T("%s\\zoom_%d%s\\%d\\%d\\%d"), lpPathName, iZoomx, szTSuffix, i, j, k );
                        CreateDirectory( strname, NULL );

                        for ( int m=0; m < FILE_COUNT_MAX; m++ )
                        {
                            _stprintf( strname, _T("%s\\zoom_%d%s\\%d\\%d\\%d\\%d"), lpPathName, iZoomx, szTSuffix, i, j, k, m );
                            CreateDirectory( strname, NULL );
                        }
                    }
                }
            }
        }
    }
    else
    {
        return FALSE;
    }
	return TRUE;
}

void CMapOutput::GetMapDirectory( int iZoomx, long lRow, long lCol, TCHAR *lpPathName, BOOL bTSuffix )
{
    TCHAR szTSuffix[5] = {0};
    if ( bTSuffix != NULL )
    {
        _tcscpy( szTSuffix, _T("_t") );
    }
    if ( m_nPicMapWidth == 256 )
    {
        long lRows = pow(2.0, iZoomx);
        long lPosition = lRows * lRow + lCol;
        if ( iZoomx <= 4 )
        {
            _stprintf( lpPathName, _T("\\zoom_%d%s\\0"), iZoomx, szTSuffix );
        }
        else if ( iZoomx <= 8 )
        {
            int i = lPosition / FILE_COUNT_MAX;
            _stprintf( lpPathName, _T("\\zoom_%d%s\\%d"), iZoomx, szTSuffix, i );
        }
        else if ( iZoomx <= 12 )
        {
            int i = lPosition /(FILE_COUNT_MAX * FILE_COUNT_MAX);
            int j = lPosition % (FILE_COUNT_MAX * FILE_COUNT_MAX) / FILE_COUNT_MAX;
            _stprintf( lpPathName, _T("\\zoom_%d%s\\%d\\%d"), iZoomx, szTSuffix, i, j );
        }
        else if ( iZoomx <= 16 )
        {
            int i = lPosition / (FILE_COUNT_MAX * FILE_COUNT_MAX * FILE_COUNT_MAX);
            int j = lPosition % (FILE_COUNT_MAX * FILE_COUNT_MAX * FILE_COUNT_MAX) / (FILE_COUNT_MAX * FILE_COUNT_MAX);
            int k = lPosition % (FILE_COUNT_MAX * FILE_COUNT_MAX * FILE_COUNT_MAX) % (FILE_COUNT_MAX * FILE_COUNT_MAX) / FILE_COUNT_MAX;
            _stprintf( lpPathName, _T("\\zoom_%d%s\\%d\\%d\\%d"), iZoomx, szTSuffix, i, j, k );
        }
        else if ( iZoomx <= 20 )
        {
            INT64 i64Rows = pow( 2.0, iZoomx );
            INT64 i64Position = i64Rows * lRow + lCol;
            INT64 i64CountMax = FILE_COUNT_MAX * FILE_COUNT_MAX;
            i64CountMax *= FILE_COUNT_MAX * FILE_COUNT_MAX;
            INT64 i = i64Position / i64CountMax;
            INT64 j = i64Position % (i64CountMax) / (FILE_COUNT_MAX * FILE_COUNT_MAX * FILE_COUNT_MAX);
            INT64 k = i64Position % (i64CountMax) % (FILE_COUNT_MAX * FILE_COUNT_MAX * FILE_COUNT_MAX) / (FILE_COUNT_MAX * FILE_COUNT_MAX);
            INT64 m = i64Position % (i64CountMax) % (FILE_COUNT_MAX * FILE_COUNT_MAX * FILE_COUNT_MAX) % (FILE_COUNT_MAX * FILE_COUNT_MAX) / FILE_COUNT_MAX;
            TCHAR buffer[4][32] = { {0} };
            _i64tot( i, buffer[0], 10 );
            _i64tot( j, buffer[1], 10 );
            _i64tot( k, buffer[2], 10 );
            _i64tot( m, buffer[3], 10 );
            _stprintf( lpPathName, _T("\\zoom_%d%s\\%s\\%s\\%s\\%s"), iZoomx, szTSuffix, buffer[0], buffer[1], buffer[2], buffer[3] );
        }
    }
}






//////////////////////////////////////////////////////////////////////////
// 

BOOL CMapOutput::Create_Directory(LPCTSTR lpPathName)
{
    BOOL T = FALSE;
    int nMode = 0;
    CString strDir = lpPathName;
    strDir.TrimRight(_T('\\'));
    strDir.TrimRight(_T('/'));
    int nPos0 = strDir.ReverseFind(_T('\\'));
    int nPos1 = strDir.ReverseFind(_T('/'));
    int nPos = nPos0 > nPos1 ? nPos0 : nPos1;
    strDir = strDir.Left(nPos);
    if(nPos>2)
    {
#ifdef UNICODE
        nMode  =_waccess(strDir, 0);
#else
        nMode = _access(strDir, 0);
#endif
        if(nMode == -1)
        {
            T = Create_Directory(strDir);
        }
    }
    else
    {
        T = TRUE;
    }
    T = CreateDirectory(lpPathName,NULL);
    return T;
}

BOOL CMapOutput::IsDirectoryExist( LPCTSTR lpPathName )
{
    if ( _taccess( lpPathName, 0 ) == -1 )
    {
        return FALSE;
    }
    return TRUE;
}
