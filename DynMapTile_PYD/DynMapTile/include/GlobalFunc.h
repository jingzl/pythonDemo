
#pragma once

#include "Platform.h"
#include "stCommon.h"
#include "trigonfun.dat"

#define break_if(x)		if ( (x) ) { break; }

// �ڴ��ͷ�
#define SAFE_DELETE(p) \
	if (p) \
	{ \
	delete p; \
	p = NULL; \
	}

#define SAFE_DELETEARY(p) \
	if (p) \
	{ \
	delete[] p; \
	p = NULL; \
	}

#define SAFE_FREE(p) \
	if (p) \
	{ \
	free(p); \
	p = NULL; \
	}

#define SIN(x) ((ZINT32)(x)<0?SIN_LOOK[(ZINT32(x)+360)]:SIN_LOOK[ZINT32(x)%360])
#define COS(x) ((ZINT32)(x)<0?COS_LOOK[(ZINT32(x)+360)]:COS_LOOK[ZINT32(x)%360])

#define GETMIN(n1, n2, n3, n4) \
	(n1>n2?(n2>n3?(n3>n4?n4:n3):(n2>n4?n4:n2)):(n1>n3?(n3>n4?n4:n3):(n1>n4?n4:n1)))

#define GETMAX(n1, n2, n3, n4) \
	(n1<n2?(n2<n3?(n3<n4?n4:n3):(n2<n4?n4:n2)):(n1<n3?(n3<n4?n4:n3):(n1<n4?n4:n1)))

#define CPLOBYTE(w) ((ZBYTE)((ZDWORD)(w) & 0xff))
#define CPHIBYTE(w) ((ZBYTE)((ZDWORD)(w) >> 8))

#define ZMAX(a,b)			(((a) > (b)) ? (a) : (b))
#define ZMIN(a,b)			(((a) < (b)) ? (a) : (b))

#define DISTANCE(PT1, PT2)  (sqrt((ZFLOAT64) ( ( (PT1).x-(PT2).x)*((PT1).x-(PT2).x) ) + ( ( (PT1).y-(PT2).y)*((PT1).y-(PT2).y)) ))

// ���þ���
#define SETRECT( rc, _left, _top, _right, _bottom ) { \
	(rc).left = (_left);  (rc).top = (_top); \
	(rc).right = (_right);  (rc).bottom = (_bottom); \
}

#define TORECT( rc, x, y, width, height ) { \
	(rc).left = (x);  (rc).top = (y); \
	(rc).right = (x) + (width);  (rc).bottom = (y) + (height); \
}
#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lParam)	((int)(short)LOWORD(lParam))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lParam)	((int)(short)HIWORD(lParam))
#endif

#define ZMAKEINTRESOURCE(i) ((ZCHAR*)((ZUINT32)((ZWORD)(i))))

#define ZRGB(r,g,b)         ((ZCOLORREF)(((ZUINT8)(r)|((ZUINT16)((ZUINT8)(g))<<8))|(((ZUINT32)(ZUINT8)(b))<<16)))
//#define ZRGB(r,g,b)			((ZCOLORREF)((0xFF000000) | (ZMIN((b), 255) << 16) | (ZMIN((g), 255) << 8) | (ZMIN((r), 255))))
//#define ZRGBA(r,g,b,a)		((ZCOLORREF)((ZMAX(0, ZMIN((a), 255)) << 24) | (ZMAX(0, ZMIN((b), 255)) << 16) | (ZMAX(0, ZMIN((g), 255)) << 8) | (ZMAX(0, ZMIN((r), 255)))))

#define ZGetRValue(clr)		( (clr)        & 0xff)
#define ZGetGValue(clr)		(((clr) >>  8) & 0xff)
#define ZGetBValue(clr)		(((clr) >> 16) & 0xff)
#define ZGetAValue(clr)		(((clr) >> 24) & 0xff)

ZVOID GetRGBA(ZCOLORREF clr, ZINT32& r, ZINT32& g, ZINT32& b, ZINT32& a);

ZVOID CPSetLastError(ZDWORD dwErrCode);
ZDWORD CPGetLastError();

ZBOOL ComputeSegmentAngle(ZPOINT* p1, ZPOINT* p2, ZINT32* piAngle );

ZINT32 WherePtLiesSegment( const ZPOINT* p, const ZPOINT* p1, const ZPOINT* p2 );

ZFLOAT64 Distance(ZFLOAT64 nX1, ZFLOAT64 nY1, ZFLOAT64 nX2, ZFLOAT64 nY2);

ZINT32 DistOfP2L(ZINT32 nX, ZINT32 nY, ZINT32 nOrgX, ZINT32 nOrgY, ZINT32 nEndX, ZINT32 nEndY, ZPOINT& ptUp, ZBOOL* pbOnLine = NULL);

ZVOID LLDistance(ZFLOAT64 dLong1, ZFLOAT64 dLat1, ZFLOAT64 dLong2, ZFLOAT64 dLat2, ZFLOAT64 *angle, ZFLOAT64 *length);


ZPOINT ShortToLongPoint(ZDWORD dwMapID, SHORTPOINT &stPoint);


ZBOOL PointInRect(const ZRECT* lprc, ZPOINT pt);


// ���ַ�תխ�ַ�(UTF8)
ZCHAR* WTOA(const ZTCHAR* pSrc);

// խ�ַ�(UTF8)ת���ַ�
ZTCHAR* ATOW(const ZCHAR* pSrc);
// GB2312ת���ַ�
ZTCHAR* GTOU(const ZCHAR* pSrc);
// ���ַ�תGB2312
ZCHAR* UTOG(const ZTCHAR* pSrc);
// GB2312תUTF8
ZCHAR* GTOUTF8(const ZCHAR* pSrc);
// UTF8תGB2312
ZCHAR* UTF8TOG(const ZCHAR* pSrc);

// »ñÈ¡MapID
/***********************************************************************************/
//  º¯Êý£º »ñÈ¡MapID
//  ²ÎÊý£º [in]  & iLinkID      ¹æ»®Â·Ïß        
//  ·µ»ØÖµ£ºMapID
//  ±ž×¢£º iLinkIDÇ°6Î»ÎªžÃLinkËùÔÚµ×²ãÍŒ·ùµÄËÄÎ¬ÍŒ·ùºÅ£¬žÃµ×²ãÍŒ·ùÓÃÓÚÉú³ÉÒýµŒÂ·Ÿ¶
/***********************************************************************************/
ZDWORD LinkIDToMapID(ZLONGLONG &iLinkID);
