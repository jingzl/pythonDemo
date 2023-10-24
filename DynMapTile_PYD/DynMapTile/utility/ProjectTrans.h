//////////////////////////////////////////////////////////////////////////
//
// CProjectTrans：基于WGS84大地基准，Spherical Mercator投影方式的坐标转换。
// 依据GOOGLE地图的投影处理方式，以世界范围为基准，图片尺寸为256像素。
//
//////////////////////////////////////////////////////////////////////////
#pragma once
#define min(a, b)  (((a) < (b)) ? (a) : (b))
#define max(a, b)  (((a) > (b)) ? (a) : (b))

const double EarthRadius = 6378137;
// 世界范围
const double MinLatitude = -85.05112878;
const double MaxLatitude = 85.05112878;
const double MinLongitude = -180;
const double MaxLongitude = 180;
//Tile宽高(pixel)
const int TileWidth = 256;
const int TileHeight = 256;


class CProjectTrans 
{
public:
    CProjectTrans();
    ~CProjectTrans();

    /**
    * @param latitude    纬度
    * @param longitude   经度
    * @param zoom 缩放等级
    * @return  
    * 功能：转换经纬度坐标(WGS-84)为指定缩放等级下的像素坐标
    */
    void LongLatToPixelXY(double longitude, double latitude, int zoom, int& pixelX, int& pixelY);

    /**
    * @param pixelX    X方向像素坐标值
    * @param pixelY    Y方向像素坐标值
    * @param zoom 缩放等级
    * @return  经纬度坐标值
    * 功能：指定缩放等级下的像素坐标转换为经纬度坐标(WGS-84)
    */
    void PixelXYToLongLat(int pixelX, int pixelY, int zoom, double& longitude, double& latitude);

    /**
    * @param pixelX  X方向像素坐标值
    * @param pixelY  Y方向像素坐标值
    * @return  瓦片(Tile)坐标值
    * 功能：像素坐标转换为瓦片(Tile)坐标
    */
    void pixelXYToTileXY(int pixelX, int pixelY, int& tileX, int& tileY, bool& bBoundaryX, bool& bBoundaryY);

    /**
    * @param tileX   X方向的瓦片(Tile)坐标值
    * @param tileY   Y方向的瓦片(Tile)坐标值
    * @return  瓦片(Tile)左上角的像素坐标值
    * 功能：转换指定瓦片(Tile)的左上角的坐标值为像素坐标值
    */
    void tileXYToPixelXY(int tileX, int tileY, int& pixelX, int& pixelY);


    /*
    * @param tileX   X方向的瓦片(Tile)坐标值
    * @param tileY   Y方向的瓦片(Tile)坐标值
    * @param zoom   缩放等级  
    * @return  瓦片(Tile)的经纬度范围
    * 功能：转换指定瓦片(Tile)的经纬度范围
    */
    void tileXYToLLBound(int tileX, int tileY, int zoom, double& minLon, double& maxLat, double& maxLon, double& minLat);


protected:

    /**
    * @param minValue  下边界值
    * @param maxValue  上边界值
    * 功能：若指定值落在指定范围内，取该值；若指定值小于下边界，取下边界值；若指定值大于上边界，取上边界值
    */
    static double Clip(double n, double minValue, double maxValue)
    {
        return min(max(n, minValue), maxValue);
    }

    /**
    * @param zoom 缩放等级
    * @return  指定缩放等级下，地图的最大宽度和高度(像素值)
    * 功能：计算指定缩放等级的地图的宽和高(像素值)
    */
    static int MapSize(int zoom)
    {
        return 256 << zoom;
    }


private:


};

