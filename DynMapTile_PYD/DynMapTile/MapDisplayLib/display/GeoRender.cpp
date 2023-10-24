#include "StdAfx.h"
#include "GeoRender.h"
#include <math.h>

CGeoRender::CGeoRender()
{

}

CGeoRender::~CGeoRender()
{

}


//��������: �������ƶ����
//�������: pCanvas, ��ʾ����; 
//			pPnt,Ҫ��ʾ����Ļ�� 
//			nCode,���Ŵ���
//			pStyle,���������
//�������: ��
//���ؽ��: ��
void CGeoRender::Polygon(CCanvas* pCanvas,
							   const POINT* pPnts, int nCount , CGeoPolygonStyle& style)
{
	switch (style.m_drawtype)
	{
	case eAREA_DT_SOLID:
		pCanvas->Polygon(pPnts, nCount );
		break;
	case eAREA_DT_BMP:
		break;
	}
}

//�ֶλ��Ʋ������зֵ����ṹ
typedef struct stClipPnt
{
	int		index;//��Ҫ�зֵĵ������λ��
	POINT	p;		//�зֲ����ĵ�
} ClipPnt;

//����䳤�ȼ���
int CGeoRender::LENGTH(int DX, int DY)  
{
	return sqrt( double(DX*DX)+(DY*DY) );  
}

void CGeoRender::Polyline(CCanvas* pCanvas,
			  const POINT* pPnts, int nCount, CGeoPolylineStyle& style)
{
	switch (style.m_drawtype)
	{
	case eLINE_DT_RAILWAY:
        {
            // ��ϡ
            int iRet = 0;
            PolySimplifyInt2D( 625, (POINT*)pPnts, nCount, &iRet );
		    DrawAlternatingLine(pCanvas, pPnts, iRet, &style);
		    break;
        }
	case eLINE_DT_SINGLE:
	case eLINE_DT_DOUBLE:
		pCanvas->Polyline(pPnts, nCount);		
		break;
	}	
}
void CGeoRender::DrawPoint(CCanvas* pCanvas,
					  const POINT* pPnt, CGeoPointStyle* style)
{
	switch (style->m_drawtype)
	{
	case ePOINT_DT_TRUETYPE:    // TrueType����
		{
			RECT r;
			r.left   = pPnt->x-style->m_nWidth/2;
			r.top    = pPnt->y-style->m_nHeight/2;
			r.right  = pPnt->x+style->m_nWidth/2;
			r.bottom = pPnt->y+style->m_nHeight/2;
			::DrawText(pCanvas->GetDC(), (LPCWSTR)&style->m_nSubCode, 1, &r, DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		}
		break;
	case ePOINT_DT_ICO:      // ICOͼ��
	case ePOINT_DT_BMP:      // BMPͼƬ
        {
            // Ŀǰ��֧��
            break;
        }
	case ePOINT_DT_PNG:      // PNGͼƬ
        {
            CPngImg* pImage = style->GetImage();
            if ( pImage != NULL )
            {
                pImage->Draw( pCanvas->GetDC(), pPnt->x-style->m_nWidth/2, pPnt->y-style->m_nHeight/2, style->m_nWidth, style->m_nHeight,
                    0, 0, pImage->GetWidth(), pImage->GetHeight() );
            }
		    break;
        }
	}
}


//��������: �����������
//�������: pCanvas, ��ʾ����; 
//			pPnts,Ҫ��ʾ����Ļ������; 
//			nCount,��������Ŀ;
//			pStyle,���������
//�������: ��
//���ؽ��: ��
void CGeoRender::DrawAlternatingLine(
									   CCanvas* pCanvas,
									   const POINT* pPnts, 
									   int& nCount, 
									   CGeoPolylineStyle* pStyle )
{
	POINT m_Pnts[3];
	ClipPnt startClipPnt, endClipPnt;
	startClipPnt.index = -1;
	startClipPnt.p = pPnts[0];

	int lenth;
	float x,y;

	if(pStyle->m_EdgeWidth > 0)
	{
		pCanvas->SetGdiPen(&pStyle->m_EdgePen1);
		pCanvas->Polyline(pPnts, nCount);
	}
	for(int i=1; i<nCount; i++)
	{
		lenth = LENGTH(pPnts[i].x-pPnts[i-1].x, pPnts[i].y-pPnts[i-1].y);

		if( (pStyle->m_bAlter == TRUE && pStyle->m_Lastdis+lenth < pStyle->m_Length1 ) 
			|| (pStyle->m_bAlter == FALSE && pStyle->m_Lastdis+lenth < pStyle->m_Length2 ) 
			|| (lenth == 0) )
		{
			pStyle->m_Lastdis += lenth;
			continue;
		}

		pStyle->m_Lastdis += lenth;

		while(TRUE)
		{
			if(pStyle->m_bAlter == TRUE)
			{//���Ƶ�һ����
				pCanvas->SetGdiPen(&pStyle->m_FillPen1);
				x = (pPnts[i].x-pPnts[i-1].x) * (lenth + pStyle->m_Length1 - pStyle->m_Lastdis);
				x /= lenth;
				y = (pPnts[i].y-pPnts[i-1].y) * (lenth + pStyle->m_Length1 - pStyle->m_Lastdis);
				y /= lenth;
				endClipPnt.index = i-1; //��¼ǰ��λ��
				endClipPnt.p.x = pPnts[i-1].x + x;
				endClipPnt.p.y = pPnts[i-1].y + y;
				if(i-startClipPnt.index > 2)
				{
					m_Pnts[0] = startClipPnt.p;
					m_Pnts[1] = pPnts[startClipPnt.index+1];
					m_Pnts[2] = pPnts[startClipPnt.index+2];
					pCanvas->Polyline(m_Pnts, 3);
					pCanvas->Polyline(&pPnts[startClipPnt.index+1], i-startClipPnt.index-1);
					m_Pnts[0] = pPnts[i-2];
					m_Pnts[1] = pPnts[i-1];
					m_Pnts[2] = endClipPnt.p;
					pCanvas->Polyline(m_Pnts, 3);
				}
				else if(i-startClipPnt.index == 2)
				{
					m_Pnts[0] = startClipPnt.p;
					m_Pnts[1] = pPnts[startClipPnt.index+1];
					m_Pnts[2] = endClipPnt.p;
					pCanvas->Polyline(m_Pnts, 3);
				}
				else
				{
					m_Pnts[0] = startClipPnt.p;
					m_Pnts[1] = endClipPnt.p;
					pCanvas->Polyline(m_Pnts, 2);
					//
				}
				pStyle->m_bAlter = !pStyle->m_bAlter;
				startClipPnt = endClipPnt;
				pStyle->m_Lastdis -= pStyle->m_Length1;
				if(pStyle->m_Lastdis < pStyle->m_Length2)
				{
					break;
				}
			}
			else
			{//���Ƶ�һ����
				pCanvas->SetGdiPen(&pStyle->m_FillPen2);
				x = (pPnts[i].x-pPnts[i-1].x) * (lenth + pStyle->m_Length2 - pStyle->m_Lastdis);
				x /= lenth;
				y = (pPnts[i].y-pPnts[i-1].y) * (lenth + pStyle->m_Length2 - pStyle->m_Lastdis);
				y /= lenth;
				endClipPnt.index = i-1; //��¼ǰ��λ��
				endClipPnt.p.x = pPnts[i-1].x + x;
				endClipPnt.p.y = pPnts[i-1].y + y;
				if(i-startClipPnt.index > 2)
				{
					m_Pnts[0] = startClipPnt.p;
					m_Pnts[1] = pPnts[startClipPnt.index+1];
					m_Pnts[2] = pPnts[startClipPnt.index+2];
					pCanvas->Polyline(m_Pnts, 3);
					pCanvas->Polyline(&pPnts[startClipPnt.index+1], i-startClipPnt.index-1);
					m_Pnts[0] = pPnts[i-2];
					m_Pnts[1] = pPnts[i-1];
					m_Pnts[2] = endClipPnt.p;
					pCanvas->Polyline(m_Pnts, 3);
				}
				else if(i-startClipPnt.index == 2)
				{
					m_Pnts[0] = startClipPnt.p;
					m_Pnts[1] = pPnts[startClipPnt.index+1];
					m_Pnts[2] = endClipPnt.p;
					pCanvas->Polyline(m_Pnts, 3);
				}
				else
				{
					m_Pnts[0] = startClipPnt.p;
					m_Pnts[1] = endClipPnt.p;
					pCanvas->Polyline(m_Pnts, 2);
					//
				}
				pStyle->m_bAlter = !pStyle->m_bAlter;
				startClipPnt = endClipPnt;
				pStyle->m_Lastdis -= pStyle->m_Length2;
				if(pStyle->m_Lastdis < pStyle->m_Length1)
				{
					break;
				}
			}
		}//while
	} // for
	if(pStyle->m_Lastdis > 0)
	{//����ʣ��Ĳ���
		if(pStyle->m_bAlter == TRUE)
		{//���Ƶ�һ����
			pCanvas->SetGdiPen(&pStyle->m_FillPen1);
			m_Pnts[0] = startClipPnt.p;
			m_Pnts[1] = pPnts[nCount-1];
			pCanvas->Polyline(m_Pnts, 2);
		}
		else
		{//���Ƶ�һ����
			pCanvas->SetGdiPen(&pStyle->m_FillPen2);
			m_Pnts[0] = startClipPnt.p;
			m_Pnts[1] = pPnts[nCount-1];
			pCanvas->Polyline(m_Pnts, 2);
		}
	}
}