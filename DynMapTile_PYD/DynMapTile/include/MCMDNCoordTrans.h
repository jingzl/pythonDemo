#ifndef _MCMDNCOORDTRANS_H_
#define _MCMDNCOORDTRANS_H_
#include "MCMDNDataDef.h"


class CMCMDNCoordTrans 
{
public:
    CMCMDNCoordTrans();
    ~CMCMDNCoordTrans();


    // 获得CMCMDNLevel标识
    static char GetMCMDNLevel( int nIdx );

    // 获得文件路径
    static void GetMCMDNFilePath( int level, int meshX, int meshY, char* szFile );
    static void GetMCMDNBlockFilePath( int level, int meshX, int meshY, int blockX, int blockY, char* szFile );

    // 根据经纬度获得MeshXY
    static void ZNMapGetMeshXYOfLL( int level, int lon, int lat, int& meshX, int& meshY );

    // 根据MeshXY获得下属BlockXY的索引
    static int ZNMapGetBlockIdxOfMesh( int meshX, int meshY, int blockX, int blockY );

    // 根据经纬度获得BlockXY
    static void ZNMapGetXYOfLL( int level, int lon, int lat, int& blockX, int& blockY );

    // 根据BlockXY获得经纬度范围
    static void ZNMapGetLLOfBlockRect( int level, int blockX, int blockY, int& minX, int& minY, int& maxX, int& maxY );

    // 根据经纬度计算BlockXY内的坐标
    static void ZNMapGetBlockXYOfLL( int level, int blockX, int blockY, int lon, int lat, int& x_block, int& y_block );




protected:



};

#endif
