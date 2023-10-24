//
//  ZNGeometry.h
//  ZDCNavi
//
//  Created by Satoshi Muraki on 08/07/30.
//  Copyright 2008 SONORAN BLUE. All rights reserved.
//
 
/*
    ZNGeometry.h
*/
#pragma once
#include "DataType.h"
#include <limits>
#include <math.h>

#define M_E         2.71828182845904523536028747135266250   /* e              */
#define M_LOG2E     1.44269504088896340735992468100189214   /* log2(e)        */
#define M_LOG10E    0.434294481903251827651128918916605082  /* log10(e)       */
#define M_LN2       0.693147180559945309417232121458176568  /* loge(2)        */
#define M_LN10      2.30258509299404568401799145468436421   /* loge(10)       */
#define M_PI        3.14159265358979323846264338327950288   /* pi             */
#define M_PI_2      1.57079632679489661923132169163975144   /* pi/2           */
#define M_PI_4      0.785398163397448309615660845819875721  /* pi/4           */
#define M_1_PI      0.318309886183790671537767526745028724  /* 1/pi           */
#define M_2_PI      0.636619772367581343075535053490057448  /* 2/pi           */
#define M_2_SQRTPI  1.12837916709551257389615890312154517   /* 2/sqrt(pi)     */
#define M_SQRT2     1.41421356237309504880168872420969808   /* sqrt(2)        */
#define M_SQRT1_2   0.707106781186547524400844362104849039  /* 1/sqrt(2)      */

//added by wang.ss
#define ZN_DEGREE_180      648000000

//end by wang.ss
struct ZNPoint
{
    ZINT32     x;
    ZINT32     y;
};
typedef struct ZNPoint ZNPoint;

struct ZNFloatPoint
{
	ZFLOAT32 x;
	ZFLOAT32 y;
};
typedef struct ZNFloatPoint ZNFloatPoint;

struct ZNFloatSize
{
	ZFLOAT32 width;
	ZFLOAT32 height;
};
typedef struct ZNFloatSize ZNFloatSize;

struct ZNSize
{
    ZUINT32    width;
    ZUINT32    height;
};
typedef struct ZNSize ZNSize;

/*
    @struct     ZNRect
    @abstract   
    @field     
    @field     
*/
struct ZNRect
{
    ZNPoint     origin;
    ZNSize      size;
};
typedef struct ZNRect ZNRect;

/*
    @type       ZNDirection
    @abstract 
*/
typedef ZFLOAT32 ZNDirection;

extern const ZNPoint ZNPointNull;
extern const ZNSize ZNSizeNull;
extern const ZNRect ZNRectNull;
extern const ZNRect ZNRectInfinite;
extern const ZNDirection ZNDirectionNull;

static inline ZNPoint
ZNPointMake(ZINT32 longitude, ZINT32 latitude)
{
    ZNPoint loc = { longitude, latitude };
    return loc;
}

static inline bool
ZNPointIsInvalid(ZNPoint point)
{
    return (point.x == INT_MAX || point.x == INT_MIN ||
            point.y == INT_MAX || point.y == INT_MIN);
}

static inline bool
ZNPointEqualToPoint(ZNPoint pt1, ZNPoint pt2)
{
    return (pt1.x == pt2.x && pt1.y == pt2.y);
}

static inline ZNSize
ZNSizeMake(ZUINT32 width, ZUINT32 height)
{
    ZNSize size = { width, height };
    return size;
}
//added by wang.ss for TW_67 at 2010-03-23

static inline ZNPoint
ZNPointStandardize(ZNPoint point)
{
    ZNPoint result;
    result.x = point.x % ZN_DEGREE_180;
    result.y = point.y % ZN_DEGREE_180;
    return result;
}

static inline ZNPoint
ZNPointOffset(ZNPoint point, ZINT32 dx, ZINT32 dy)
{
    ZNPoint result;
    result.x = point.x + dx;
    result.y = point.y + dy;
    return result;
}

//end by wang.ss
static inline bool
ZNSizeIsInvalid(ZNSize size)
{
    return (size.width == UINT_MAX || size.height == UINT_MAX);
}

static inline ZNRect
ZNRectMake(ZINT32 x, ZINT32 y, ZUINT32 width, ZUINT32 height)
{
    ZNRect rect = { x, y, width, height };
    return rect;
}

static inline ZBOOL
ZNRectIsNull(ZNRect rect)
{
    return (rect.origin.x == ZNRectNull.origin.x &&
            rect.origin.y == ZNRectNull.origin.y &&
            rect.size.width == ZNRectNull.size.width && 
            rect.size.height == ZNRectNull.size.height);
}

static inline ZBOOL
ZNRectIsInfinite(ZNRect rect)
{
    return (rect.origin.x == ZNRectInfinite.origin.x &&
            rect.origin.y == ZNRectInfinite.origin.y &&
            rect.size.width == ZNRectInfinite.size.width &&
            rect.size.height == ZNRectInfinite.size.height);
}

static inline ZBOOL
ZNRectIntersectsRect(ZNRect rect1, ZNRect rect2)
{
    return (!((rect1.origin.x + rect1.size.width < rect2.origin.x) ||
              (rect2.origin.x + rect2.size.width < rect1.origin.x) ||
              (rect1.origin.y + rect1.size.height < rect2.origin.y) ||
              (rect2.origin.y + rect2.size.height < rect1.origin.y)));
}

static inline ZBOOL
ZNRectContainsPoint(ZNRect rect, ZNPoint point)
{
    return (rect.origin.x <= point.x &&
            point.x < rect.origin.x + rect.size.width &&
            rect.origin.y <= point.y &&
            point.y < rect.origin.y + rect.size.height);
}

static inline ZNPoint
ZNRectGetCenter(ZNRect rect)
{
    ZNPoint point;
    point.x = rect.origin.x + (rect.size.width / 2);
    point.y = rect.origin.y + (rect.size.height / 2);
    return point;
}

static inline ZBOOL
ZNDirectionIsInvalid(ZNDirection direction)
{
    return (direction < 0.0);
}

static inline ZNDirection
ZNDirectionStandardize(ZFLOAT32 direction)
{
    ZNDirection newDirection = direction;
    while (newDirection < 0.0)
        newDirection += 360.0;
    while (360.0 <= newDirection)
        newDirection -= 360.0;
    return newDirection;
}

extern ZNDirection
ZNDirectionFromVector(ZNPoint v);

extern double
ZNPointsDistance(ZNPoint point1, ZNPoint point2);

extern ZUINT32
ZNDistanceFromMeter(double meter);

extern 
ZUINT32 round(double r);

extern void
ZNGeometryLLH2XYZ(double latitude, double longitude, double height, double a, double e2, double *outX, double *outY, double *outZ);

extern void
ZNGeometryXYZ2LLH(double x, double y, double z, double a, double e2, double *outLat, double *outLong, double *outHeight);
