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



// 每个文件夹下存放的最大文件数
#define FILE_COUNT_MAX          256

// 透明色
const RGBQUAD CN_TRANS_COLOR = {255,255,255,0};


class CMapOutput : public IMapOutput 
{
public:
	static CMapOutput* Instance(void);
	~CMapOutput(void);

public:

	// 设置日志
	void SetLogFunc( LogFunc lplogfunc );

    //初始化
    BOOL InitMapOutput( IMapDisplay* pMapDisplay, IParser* pParser, const char* lpszPicPath, const char* lpszTxtPath );

	//地图输出
	BOOL OutputMap( int iZoomx, double Xmin, double Ymin, double Xmax, double Ymax, BYTE byteBigDisplayMode=0 );

    // 释放
    void Release();

protected:
    CMapOutput(void);

    void OutputMessage( CString str );

    //////////////////////////////////////////////////////////////////////////
    // 
    // 按某比例尺下的某块区域切图
    BOOL CutMapByRect( int iZoomx, double Xmin, double Ymin, double Xmax, double Ymax );

    // 按某比例尺下的行列范围切图
    BOOL CutMapByRC( int iZoomx, int mini, int minj, int maxi, int maxj );

    // 地图绘制
    BOOL DrawCutMap( int iZoomx, GEOPOINT pos, PTILETEXT_INFO pTileTextInfo );

	//函数功能: 输出对应DC上的RECT区域位图
	//输入参数: hScrDC, GIS设备DC; 
	//          lpRect, 剪切的区域;
	//输出参数: HBITMAP
	//返回结果: HBITMAP	 
	void CopyDCToBitmap( LPRECT lpRect );

	//函数功能: 按imagetype生产对应类型的图片
	//输入参数: lpPathName, 输出路径;
	//          hBitmap, 位图句柄;
	//          iZoomx,  地图等级;
	//          lRow,    横坐标;
	//			lCol,	 纵坐标;
	//输出参数: 无
	//返回结果: 无
	BOOL CreateXimages( int iZoomx, long lRow, long lCol, DWORD imagetype );

    // 输出Tile中的注记热区信息
    BOOL CreateTileTextFile( int iZoomx, long lRow, long lCol, GEORECT& rect, PTILETEXT_INFO pTileTextInfo );

	//函数功能: 按地图等级生产所有文件夹
	//输入参数: lpPathName, 输出路径;	
	//          iZoomx,  地图等级;	
	//输出参数: 无
	//返回结果: 创建文件的返回码	
	BOOL CreateMapDirectory( LPCTSTR lpPathName, int iZoomx, BOOL bTSuffix=FALSE );

	//函数功能: 按地图等级及纵横坐标位置输出该图片对应文件路径
	//输入参数: 
	//          iZoomx,  地图等级;
	//          lRow,    横坐标;
	//			lCol,	 纵坐标;
	//输出参数: lpPathName
	//返回结果: lpPathName, 输出路径;
	// 按地图等级及纵横坐标位置输出该图片对应文件路径
	void GetMapDirectory( int iZoomx, long lRow, long lCol, TCHAR *lpPathName, BOOL bTSuffix=FALSE );

    BOOL Create_Directory( LPCTSTR lpPathName );
    BOOL IsDirectoryExist( LPCTSTR lpPathName );

private:
    static CMapOutput* m_sinstance;

    // 日志
    LogFunc m_lpLogFunc;

    // 大显示模式，如：白天，黑夜...
    BYTE m_nBigDisplayMode;

    //地图解析接口
    IParser* m_pParser;

    //地图绘制接口
	IMapDisplay* m_pMapDisplay;

    // 地图输出路径
    TCHAR m_szOutputMapPath[MAX_PATH];
    // 文字输出路径
    TCHAR m_szOutputTxtPath[MAX_PATH];

    // 屏幕和内存设备描述表
    // 绘图
    HDC m_hDC;
    // 切图
    HDC m_hMemDC;
    // 位图句柄
    HBITMAP m_hBitmap;
    HBITMAP m_hOldBitmap;
    // 选定区域坐标
    RECT m_Rect;
    // 地图范围
    GEORECT m_rcGeoRect;
    // 整图范围
    GEORECT m_rcAllMapGeoRect;

    // 地图数据对象
    PMAP_DISP_DATA m_lpMapDispData;

    // 图片尺寸
    int m_nPicMapWidth;
    int m_nPicPoiWidth;

    // CxImage对象
    CxImage m_Cximage;

    // 投影变换
    CProjectTrans m_projectTrans;

    // TileTextWriter
    CTileTextWriter m_TileTextWriter;

    // 文件名称
    TCHAR m_szFilename[MAX_PATH];
    // 文件路径
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

