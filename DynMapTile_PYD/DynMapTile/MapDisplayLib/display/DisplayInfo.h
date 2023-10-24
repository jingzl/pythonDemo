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
	// ��ʼ��
	BOOL Initialize(SIZE bufSize);
    // ����ͼƬ·��
	void SetImagePath(const TCHAR* lpszPath);
	// ���õ�ͼλ��
	BOOL SetMapPos(GEOPOINT pos, _tag_map_pos mapPos);
	// ���ñ�����
	BOOL SetMapScale(double dfScale, int scale);
	// �ƶ�
	BOOL MoveMap(double dfOffsetX, double dfOffsetY);
    // ��þ�γ��
    BOOL GetLLPos( POINT& pt, GEOPOINT& geopt, int& nCol, int& nRow );
    // ��Ļ����ת��γ��
    BOOL Src2Map( POINT& ptSrc, GEOPOINT& geoPt );
    // ��ȡ���ĵ�
    GEOPOINT GetCenterPos();

	// ��ȡ��ͼ��Χ
	GEORECT GetMapRect();
	// ��ȡ��ͼ������Χ
	RECT GetBufRect()
	{
		RECT rc;
		rc.left   = 0;
		rc.right  = m_bufSize.cx;
		rc.top    = 0;
		rc.bottom = m_bufSize.cy;
		return rc;
	}
	// ��ȡHDC
	HDC GetHDC();

	//��ͼ����ʾ����ת��   
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

	//�õ���ʾ�����Ӧ�ĵ�ͼ�����µĴ�С
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

    //! ��¼͸����ʾ״̬
    BOOL m_bShow3D;
    CAffineTrans m_AffineTrans;

};
