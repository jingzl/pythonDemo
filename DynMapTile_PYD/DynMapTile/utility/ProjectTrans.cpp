#include "stdafx.h"
#include "ProjectTrans.h"
#include <math.h>
#define PI  3.14159265358979323846


CProjectTrans::CProjectTrans()
{
}

CProjectTrans::~CProjectTrans()
{
}

void CProjectTrans::LongLatToPixelXY(double longitude, double latitude, int zoom, int& pixelX, int& pixelY)
{
    int mapSize = MapSize(zoom);

    longitude = Clip(longitude, MinLongitude, MaxLongitude);
    double x = (longitude + 180) / 360; 
    pixelX = (int) Clip(x * mapSize + 0.5, 0, mapSize - 1);

    latitude = Clip(latitude, MinLatitude, MaxLatitude);
    double sinLatitude = sin(latitude * PI / 180);
    double y = 0.5 - log((1 + sinLatitude) / (1 - sinLatitude)) / (4 * PI);      
    pixelY = (int) Clip(y * mapSize + 0.5, 0, mapSize - 1);
}

void CProjectTrans::PixelXYToLongLat(int pixelX, int pixelY, int zoom, double& longitude, double& latitude)
{
    double mapSize = MapSize(zoom);
    double x = (Clip(pixelX, 0, mapSize - 1) / mapSize) - 0.5;
    double y = 0.5 - (Clip(pixelY, 0, mapSize - 1) / mapSize);

    longitude = 360 * x;
    latitude = 90 - 360 * atan(exp(-y * 2 * PI)) / PI;
}

void CProjectTrans::pixelXYToTileXY( int pixelX, int pixelY, int& tileX, int& tileY, bool& bBoundaryX, bool& bBoundaryY )
{
    tileX = pixelX / TileWidth;
    tileY = pixelY / TileHeight;
    // judge boundary
    bBoundaryX = (pixelX % TileWidth == 0) ? true : false;
    bBoundaryY = (pixelY % TileHeight == 0) ? true : false;
}

void CProjectTrans::tileXYToPixelXY( int tileX, int tileY, int& pixelX, int& pixelY )
{
    pixelX = tileX * TileWidth;
    pixelY = tileY * TileHeight;
}

void CProjectTrans::tileXYToLLBound( int tileX, int tileY, int zoom, double& minLon, double& maxLat, double& maxLon, double& minLat )
{
    int px1 = 0;
    int py1 = 0;
    int px2 = 0;
    int py2 = 0;
    tileXYToPixelXY( tileX, tileY, px1, py1 );
    tileXYToPixelXY( tileX+1, tileY+1, px2, py2 );

    // 左上点
    PixelXYToLongLat( px1, py1, zoom, minLon, maxLat );
    // 右下点
    PixelXYToLongLat( px2, py2, zoom, maxLon, minLat );
}

