//////////////////////////////////////////////////////////////////////////
//
// CProjectTrans������WGS84��ػ�׼��Spherical MercatorͶӰ��ʽ������ת����
// ����GOOGLE��ͼ��ͶӰ����ʽ�������緶ΧΪ��׼��ͼƬ�ߴ�Ϊ256���ء�
//
//////////////////////////////////////////////////////////////////////////
#pragma once
#define min(a, b)  (((a) < (b)) ? (a) : (b))
#define max(a, b)  (((a) > (b)) ? (a) : (b))

const double EarthRadius = 6378137;
// ���緶Χ
const double MinLatitude = -85.05112878;
const double MaxLatitude = 85.05112878;
const double MinLongitude = -180;
const double MaxLongitude = 180;
//Tile���(pixel)
const int TileWidth = 256;
const int TileHeight = 256;


class CProjectTrans 
{
public:
    CProjectTrans();
    ~CProjectTrans();

    /**
    * @param latitude    γ��
    * @param longitude   ����
    * @param zoom ���ŵȼ�
    * @return  
    * ���ܣ�ת����γ������(WGS-84)Ϊָ�����ŵȼ��µ���������
    */
    void LongLatToPixelXY(double longitude, double latitude, int zoom, int& pixelX, int& pixelY);

    /**
    * @param pixelX    X������������ֵ
    * @param pixelY    Y������������ֵ
    * @param zoom ���ŵȼ�
    * @return  ��γ������ֵ
    * ���ܣ�ָ�����ŵȼ��µ���������ת��Ϊ��γ������(WGS-84)
    */
    void PixelXYToLongLat(int pixelX, int pixelY, int zoom, double& longitude, double& latitude);

    /**
    * @param pixelX  X������������ֵ
    * @param pixelY  Y������������ֵ
    * @return  ��Ƭ(Tile)����ֵ
    * ���ܣ���������ת��Ϊ��Ƭ(Tile)����
    */
    void pixelXYToTileXY(int pixelX, int pixelY, int& tileX, int& tileY, bool& bBoundaryX, bool& bBoundaryY);

    /**
    * @param tileX   X�������Ƭ(Tile)����ֵ
    * @param tileY   Y�������Ƭ(Tile)����ֵ
    * @return  ��Ƭ(Tile)���Ͻǵ���������ֵ
    * ���ܣ�ת��ָ����Ƭ(Tile)�����Ͻǵ�����ֵΪ��������ֵ
    */
    void tileXYToPixelXY(int tileX, int tileY, int& pixelX, int& pixelY);


    /*
    * @param tileX   X�������Ƭ(Tile)����ֵ
    * @param tileY   Y�������Ƭ(Tile)����ֵ
    * @param zoom   ���ŵȼ�  
    * @return  ��Ƭ(Tile)�ľ�γ�ȷ�Χ
    * ���ܣ�ת��ָ����Ƭ(Tile)�ľ�γ�ȷ�Χ
    */
    void tileXYToLLBound(int tileX, int tileY, int zoom, double& minLon, double& maxLat, double& maxLon, double& minLat);


protected:

    /**
    * @param minValue  �±߽�ֵ
    * @param maxValue  �ϱ߽�ֵ
    * ���ܣ���ָ��ֵ����ָ����Χ�ڣ�ȡ��ֵ����ָ��ֵС���±߽磬ȡ�±߽�ֵ����ָ��ֵ�����ϱ߽磬ȡ�ϱ߽�ֵ
    */
    static double Clip(double n, double minValue, double maxValue)
    {
        return min(max(n, minValue), maxValue);
    }

    /**
    * @param zoom ���ŵȼ�
    * @return  ָ�����ŵȼ��£���ͼ������Ⱥ͸߶�(����ֵ)
    * ���ܣ�����ָ�����ŵȼ��ĵ�ͼ�Ŀ�͸�(����ֵ)
    */
    static int MapSize(int zoom)
    {
        return 256 << zoom;
    }


private:


};

