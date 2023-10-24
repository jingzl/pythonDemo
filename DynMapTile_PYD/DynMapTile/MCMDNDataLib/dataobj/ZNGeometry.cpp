//
//  ZNGeometry.m
//  ZDCNavi
//
//  Created by Satoshi Muraki on 08/07/30.
//  Copyright 2008 SONORAN BLUE. All rights reserved.
//

#include "ZNGeometry.h"

const ZNPoint ZNPointNull = { INT_MAX, INT_MAX };
const ZNSize ZNSizeNull = { UINT_MAX, UINT_MAX };
const ZNRect ZNRectNull = { { INT_MAX, INT_MAX }, { 0, 0 } };
const ZNRect ZNRectInfinite = { { INT_MIN, INT_MIN }, { UINT_MAX, UINT_MAX } };
const ZNDirection ZNDirectionNull = -1.0;

void
ZNGeometryLLH2XYZ(double latitude, double longitude, double height, double a, double e2, double *outX, double *outY, double *outZ)
{
    double rd, b, l, sb, cb, rn;
    
    rd = M_PI / 180.0;
    b = latitude * rd;
    l = longitude * rd;
    sb = sin(b);
    cb = cos(b);
    rn = 6378137.0 / sqrt(1.0 - e2 * sb * sb);
    
    *outX = (rn + height) * cb * cos(l);
    *outY = (rn + height) * cb * sin(l);
    *outZ = (rn * (1.0 - e2) + height) * sb;
}

void
ZNGeometryXYZ2LLH(double x, double y, double z, double a, double e2, double *outLat, double *outLong, double *outHeight)
{
    double rd, bda, p, t, st, ct, b, l, sb, rn, h;
    
    rd = M_PI / 180.0;
    bda = sqrt(1.0 - e2);
    
    p = sqrt((x * x + y * y));
    t = atan2(z, p * bda);
    st = sin(t);
    ct = cos(t);
    b = atan2(z + e2 * a / bda * st * st * st, p - e2 * a * ct * ct * ct);
    l = atan2(y, x);
    sb = sin(b);
    rn = 1 / sqrt(1.0 - e2 * sb * sb);
    h = p / cos(b) - rn;
    
    *outLat = b / rd;
    *outLong = l / rd;
    *outHeight = h;
}

ZNDirection
ZNDirectionFromVector(ZNPoint v)
{
    ZNDirection dir = ZNDirectionNull;
    
    if (v.x == 0.0 && v.y == 0.0)
    {
        dir = ZNDirectionNull;
    }
    else if (v.x == 0)
    {
        dir = (v.y < 0 ? 180.0 : 0.0);
    }
    else if (v.y == 0)
    {
        dir = (v.x < 0 ? 270.0 : 90.0);
    }
    else if (0.0 <= v.x && 0.0 <= v.y)
    {
        dir = 90.0 - (atanf(v.y / v.x) * 180.0 / M_PI);
    }
    else if (v.x <= 0.0 && 0.0 <= v.y)
    {
        dir = 270.0 - (atanf(v.y / v.x) * 180.0 / M_PI);
    }
    else if (v.x <= 0.0 && v.y <= 0.0)
    {
        dir = 270.0 - (atanf(v.y / v.x) * 180.0 / M_PI);
    }
    else if (0.0 <= v.x && v.y <= 0.0)
    {
        dir = 90.0 - (atanf(v.y / v.x) * 180.0 / M_PI); 
    }
    
    return dir;
}

extern double
ZNPointsDistance(ZNPoint point1, ZNPoint point2)
{
    double startX, startY, endX, endY;
    double deltaX, deltaY;
    double distance;
    
    // 
    startX = (((double)point1.x / 3600000.0) * M_PI / 180.0);
    startY = (((double)point1.y / 3600000.0) * M_PI / 180.0);
    
    //
    endX = (((double)point2.x / 3600000.0) * M_PI / 180.0);
    endY = (((double)point2.y / 3600000.0) * M_PI / 180.0);
    
    // X方向的移动距离计算
    // 6378137.0 
    deltaX = 6378137.0 * (double)(endX - startX) * cos((double)startY);
    
    //Y方向的移动距离计算
    // 6378137.0 赤道的半径距离
    deltaY = 6378137.0 * (double)(endY - startY);
   
    distance = sqrt(deltaX * deltaX + deltaY * deltaY);
    
    return distance;
}

ZUINT32 round(double r)
{
	return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}

ZUINT32
ZNDistanceFromMeter(double meter)
{
    return round(648000000.0 * (double)meter / (6378137.0 * M_PI));
}
