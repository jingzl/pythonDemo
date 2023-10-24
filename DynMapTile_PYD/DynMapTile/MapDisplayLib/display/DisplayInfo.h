#pragma once
#include "IMapDisplay.h"
#include "Canvas.h"
#include "GeoRender.h"
#include "ProjectTrans.h"
#include "AffineTrans.h"


class CLabelAdjust;
class CPointLabelAdjust;

class CDisplayInfo
{
public:
	CDisplayInfo(void);
	~CDisplayInfo(void);
	// 初始化
	BOOL Initialize(SIZE bufSize);
    // 设置图片路径
	void SetImagePath(const TCHAR* lpszPath);
	// 设置地图位置
	BOOL SetMapPos(GEOPOINT pos, _tag_map_pos mapPos);
	// 设置比例尺
	BOOL SetMapScale(double dfScale, int scale);
	// 移动
	BOOL MoveMap(double dfOffsetX, double dfOffsetY);
    // 获得经纬度
    BOOL GetLLPos( POINT& pt, GEOPOINT& geopt, int& nCol, int& nRow );
    // 屏幕坐标转经纬度
    BOOL Src2Map( POINT& ptSrc, GEOPOINT& geoPt );
    // 获取中心点
    GEOPOINT GetCenterPos();

	// 获取绘图范围
	GEORECT GetMapRect();
	// 获取绘图画布范围
	RECT GetBufRect()
	{
		RECT rc;
		rc.left   = 0;
		rc.right  = m_bufSize.cx;
		rc.top    = 0;
		rc.bottom = m_bufSize.cy;
		return rc;
	}
	// 获取HDC
	HDC GetHDC();

	//地图向显示坐标转化   
    template <typename T> POINT Map2Display(T& lMapX, T& lMapY)
    {
        POINT ptDis={0,0};

        int x1,y1;
        m_ProjectTrans.LongLatToPixelXY(lMapX, lMapY, m_lScale, x1, y1);

        ptDis.x = x1-m_ptLB.x;
        ptDis.y = m_bufSize.cy-(m_ptLB.y-y1);

        if ( m_bShow3D )
        {
            float x = ptDis.x;
            float y = ptDis.y;
            m_AffineTrans.TransDisplayPointToAffineSpace( x, y, &ptDis );
        }

        return ptDis;
    }

    void Start3DMode();
    void End3DMode();

	//得到显示坐标对应的地图坐标下的大小
	template <typename T> double GetMapLength(T& len)
	{
		double d = 0;
		d = len / m_dfScaleFactor;
		return d;
	}

	CCanvas* GetCanvas() { return m_pCanvas; }
	POINT*   GetPointOut() { return m_pPoints; }
	POINT*   GetHalfPointOut() { return m_pPoints+409600/2; }

	CLabelAdjust* GetLabelAdjust() { return m_pLabelAdjust; };
    CPointLabelAdjust* GetPointLabelAdjust() { return m_pPointLabelAdjust; };


protected:
	BOOL ResetMap();
	BOOL InitCanvas(SIZE bufSize);

private:

	CCanvas*  m_pCanvas;
	GEORECT   m_geoRect;
	SIZE      m_bufSize;
	long      m_lScale;
	double    m_dfScaleFactor;
	double    m_dPixelPerMeter;		
    GEOPOINT  m_ptCenter;
    POINT     m_ptPixelCenter;
	GEOPOINT  m_ptLB;

	POINT*    m_pPoints;
	CLabelAdjust* m_pLabelAdjust;
    CPointLabelAdjust* m_pPointLabelAdjust;
	TCHAR     m_szImagePath[MAX_PATH];

    CProjectTrans m_ProjectTrans;

    //! 记录透视显示状态
    BOOL m_bShow3D;
    CAffineTrans m_AffineTrans;

};
