#include "stdafx.h"
#include "AffineTrans.h"


CAffineTrans::CAffineTrans()
{
	m_mx[0][0] = 0.5;
	m_mx[0][1] = -0.3375;
	m_mx[0][2] = 120.0;
	m_mx[1][0] = 0.0;
	m_mx[1][1] = -0.325;
	m_mx[1][2] = 260.0;
	m_mx[2][0] = 0.0;
	m_mx[2][1] = -0.00140625;
	m_mx[2][2] = 1.0;
    //ZLOGE(TAG_MAPDISPLAY, "----m_mx = (%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf)----", m_mx[0][0], m_mx[0][1], m_mx[0][2], m_mx[1][0], m_mx[1][1],m_mx[1][2], m_mx[2][0], m_mx[2][1], m_mx[2][2] );
}

CAffineTrans::~CAffineTrans()
{	
}

BOOL CAffineTrans::TransDisplayPointToAffineSpace(float& x, float& y, POINT* pPtDes)
{
    BOOL T = FALSE;
	if( pPtDes )
	{		
		y = 800 - y;

		double u , v, w;
		
		u = m_mx[0][0]*x + m_mx[0][1]*y + m_mx[0][2];
		v = m_mx[1][0]*x + m_mx[1][1]*y + m_mx[1][2];
		w = m_mx[2][0]*x + m_mx[2][1]*y + m_mx[2][2];
				
		if(w != 0)
		{
			pPtDes->x = int(u/w + 0.5);
			pPtDes->y = int(v/w + 0.5);
		}
		else
		{
			pPtDes->x =  int(u + 0.5);
			pPtDes->y =  int(v + 0.5);
		}

		pPtDes->y = 800 - pPtDes->y ;

		T = TRUE;
	}
	return T;
}
