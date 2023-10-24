#pragma once
#include "IMapOutput.h"
#include "ximage.h"
#ifdef _DEBUG
#pragma comment ( lib, "cximagecrtdu_d.lib" )
#else
#pragma comment ( lib, "cximagecrtdu.lib" )
#endif
#include "ProjectTrans.h"
#include "TileTextWriter.h"



// ÿ���ļ����´�ŵ�����ļ���
#define FILE_COUNT_MAX          256

// ͸��ɫ
const RGBQUAD CN_TRANS_COLOR = {255,255,255,0};


class CMapOutput : public IMapOutput 
{
public:
	static CMapOutput* Instance(void);
	~CMapOutput(void);

public:

	// ������־
	void SetLogFunc( LogFunc lplogfunc );

    //��ʼ��
    BOOL InitMapOutput( IMapDisplay* pMapDisplay, IParser* pParser, const char* lpszPicPath, const char* lpszTxtPath );

	//��ͼ���
	BOOL OutputMap( int iZoomx, double Xmin, double Ymin, double Xmax, double Ymax, BYTE byteBigDisplayMode=0 );

    // �ͷ�
    void Release();

protected:
    CMapOutput(void);

    void OutputMessage( CString str );

    //////////////////////////////////////////////////////////////////////////
    // 
    // ��ĳ�������µ�ĳ��������ͼ
    BOOL CutMapByRect( int iZoomx, double Xmin, double Ymin, double Xmax, double Ymax );

    // ��ĳ�������µ����з�Χ��ͼ
    BOOL CutMapByRC( int iZoomx, int mini, int minj, int maxi, int maxj );

    // ��ͼ����
    BOOL DrawCutMap( int iZoomx, GEOPOINT pos, PTILETEXT_INFO pTileTextInfo );

	//��������: �����ӦDC�ϵ�RECT����λͼ
	//�������: hScrDC, GIS�豸DC; 
	//          lpRect, ���е�����;
	//�������: HBITMAP
	//���ؽ��: HBITMAP	 
	void CopyDCToBitmap( LPRECT lpRect );

	//��������: ��imagetype������Ӧ���͵�ͼƬ
	//�������: lpPathName, ���·��;
	//          hBitmap, λͼ���;
	//          iZoomx,  ��ͼ�ȼ�;
	//          lRow,    ������;
	//			lCol,	 ������;
	//�������: ��
	//���ؽ��: ��
	BOOL CreateXimages( int iZoomx, long lRow, long lCol, DWORD imagetype );

    // ���Tile�е�ע��������Ϣ
    BOOL CreateTileTextFile( int iZoomx, long lRow, long lCol, GEORECT& rect, PTILETEXT_INFO pTileTextInfo );

	//��������: ����ͼ�ȼ����������ļ���
	//�������: lpPathName, ���·��;	
	//          iZoomx,  ��ͼ�ȼ�;	
	//�������: ��
	//���ؽ��: �����ļ��ķ�����	
	BOOL CreateMapDirectory( LPCTSTR lpPathName, int iZoomx, BOOL bTSuffix=FALSE );

	//��������: ����ͼ�ȼ����ݺ�����λ�������ͼƬ��Ӧ�ļ�·��
	//�������: 
	//          iZoomx,  ��ͼ�ȼ�;
	//          lRow,    ������;
	//			lCol,	 ������;
	//�������: lpPathName
	//���ؽ��: lpPathName, ���·��;
	// ����ͼ�ȼ����ݺ�����λ�������ͼƬ��Ӧ�ļ�·��
	void GetMapDirectory( int iZoomx, long lRow, long lCol, TCHAR *lpPathName, BOOL bTSuffix=FALSE );

    BOOL Create_Directory( LPCTSTR lpPathName );
    BOOL IsDirectoryExist( LPCTSTR lpPathName );

private:
    static CMapOutput* m_sinstance;

    // ��־
    LogFunc m_lpLogFunc;

    // ����ʾģʽ���磺���죬��ҹ...
    BYTE m_nBigDisplayMode;

    //��ͼ�����ӿ�
    IParser* m_pParser;

    //��ͼ���ƽӿ�
	IMapDisplay* m_pMapDisplay;

    // ��ͼ���·��
    TCHAR m_szOutputMapPath[MAX_PATH];
    // �������·��
    TCHAR m_szOutputTxtPath[MAX_PATH];

    // ��Ļ���ڴ��豸������
    // ��ͼ
    HDC m_hDC;
    // ��ͼ
    HDC m_hMemDC;
    // λͼ���
    HBITMAP m_hBitmap;
    HBITMAP m_hOldBitmap;
    // ѡ����������
    RECT m_Rect;
    // ��ͼ��Χ
    GEORECT m_rcGeoRect;
    // ��ͼ��Χ
    GEORECT m_rcAllMapGeoRect;

    // ��ͼ���ݶ���
    PMAP_DISP_DATA m_lpMapDispData;

    // ͼƬ�ߴ�
    int m_nPicMapWidth;
    int m_nPicPoiWidth;

    // CxImage����
    CxImage m_Cximage;

    // ͶӰ�任
    CProjectTrans m_projectTrans;

    // TileTextWriter
    CTileTextWriter m_TileTextWriter;

    // �ļ�����
    TCHAR m_szFilename[MAX_PATH];
    // �ļ�·��
    TCHAR m_szDirectroy[MAX_PATH];

	//Function Error
	static int m_nError;

	//set function last error
	static void SetLaseError( int nError )
	{
		m_nError = nError;
	}

	//return function last error
	static int GetLastError()
	{
		return m_nError;
	}

};

IMapOutput* GetMapOutputInstance()
{
    return CMapOutput::Instance();
}

