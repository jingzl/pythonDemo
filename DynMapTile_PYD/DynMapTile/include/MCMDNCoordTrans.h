#ifndef _MCMDNCOORDTRANS_H_
#define _MCMDNCOORDTRANS_H_
#include "MCMDNDataDef.h"


class CMCMDNCoordTrans 
{
public:
    CMCMDNCoordTrans();
    ~CMCMDNCoordTrans();


    // ���CMCMDNLevel��ʶ
    static char GetMCMDNLevel( int nIdx );

    // ����ļ�·��
    static void GetMCMDNFilePath( int level, int meshX, int meshY, char* szFile );
    static void GetMCMDNBlockFilePath( int level, int meshX, int meshY, int blockX, int blockY, char* szFile );

    // ���ݾ�γ�Ȼ��MeshXY
    static void ZNMapGetMeshXYOfLL( int level, int lon, int lat, int& meshX, int& meshY );

    // ����MeshXY�������BlockXY������
    static int ZNMapGetBlockIdxOfMesh( int meshX, int meshY, int blockX, int blockY );

    // ���ݾ�γ�Ȼ��BlockXY
    static void ZNMapGetXYOfLL( int level, int lon, int lat, int& blockX, int& blockY );

    // ����BlockXY��þ�γ�ȷ�Χ
    static void ZNMapGetLLOfBlockRect( int level, int blockX, int blockY, int& minX, int& minY, int& maxX, int& maxY );

    // ���ݾ�γ�ȼ���BlockXY�ڵ�����
    static void ZNMapGetBlockXYOfLL( int level, int blockX, int blockY, int lon, int lat, int& x_block, int& y_block );




protected:



};

#endif
