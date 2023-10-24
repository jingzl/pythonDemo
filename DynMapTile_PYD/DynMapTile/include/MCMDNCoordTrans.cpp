#include "stdafx.h"
#include "MCMDNCoordTrans.h"




CMCMDNCoordTrans::CMCMDNCoordTrans()
{
    // 
}

CMCMDNCoordTrans::~CMCMDNCoordTrans()
{
    // 
}

char CMCMDNCoordTrans::GetMCMDNLevel( int nIdx )
{
    char cLevel = 'i';
    switch ( nIdx )
    {
    case 0:
        {
            cLevel = 'g';
            break;
        }
    case 1:
        {
            cLevel = 'i';
            break;
        }
    case 2:
        {
            cLevel = 'j';
            break;
        }
    case 3:
        {
            cLevel = 'k';
            break;
        }
    case 4:
        {
            cLevel = 'l';
            break;
        }
    case 5:
        {
            cLevel = 'm';
            break;
        }
    case 6:
        {
            cLevel = 'n';
            break;
        }
    case 7:
        {
            cLevel = 'o';
            break;
        }
    case 8:
        {
            cLevel = 'p';
            break;
        }
    case 9:
        {
            cLevel = 'q';
            break;
        }
    default:
        {
            break;
        }
    }
    return cLevel;
}

void CMCMDNCoordTrans::GetMCMDNFilePath( int level, int meshX, int meshY, char* szFile )
{
    char cLevel = CMCMDNCoordTrans::GetMCMDNLevel( level );
    sprintf( szFile, ("%c00\\%04x\\%04x\\%04x%04x.mm"), cLevel, meshX/16, meshY/16, meshX/4, meshY/4 );
}

void CMCMDNCoordTrans::GetMCMDNBlockFilePath( int level, int meshX, int meshY, int blockX, int blockY, char* szFile )
{
    char cLevel = CMCMDNCoordTrans::GetMCMDNLevel( level );
    sprintf( szFile, ("%c00\\%04x\\%04x\\%04x%04x\\%04x%04x.mm"), cLevel, meshX/16, meshY/16, meshX/4, meshY/4, blockX, blockY );
}

void CMCMDNCoordTrans::ZNMapGetMeshXYOfLL( int level, int lon, int lat, int& meshX, int& meshY )
{
    meshX = (lon - MESH_BASE_COORX) / ZNMapBlockLongitude4[level] + 1;
    meshY = (lat - MESH_BASE_COORY) / ZNMapBlockLatitude4[level] + 1;
}

int CMCMDNCoordTrans::ZNMapGetBlockIdxOfMesh( int meshX, int meshY, int blockX, int blockY )
{
    // 行 优先
    int nIdx = 0;

    //nIdx = blockX % meshX + 4 * ( blockY % meshY );
    //return nIdx;

    int i=0;
    for ( int bXi=meshX*4; bXi<meshX*4+4; bXi++ )
    {
        int j = 0;
        for ( int bYi=meshY*4; bYi<meshY*4+4; bYi++ )
        {
            if ( bXi==blockX && bYi==blockY )
            {
                nIdx = i + j*4;
                return nIdx;
            }
            j++;
        }
        i++;
    }
    return -1;
}

void CMCMDNCoordTrans::ZNMapGetXYOfLL( int level, int lon, int lat, int& blockX, int& blockY )
{
    int a = lon - MESH_BASE_COORX;
    //blockX = ( (a / ZNMapBlockLongitude4[level]) << 2 ) + ((int)(a * 4 / ZNMapBlockLongitude4[level]) & 0x03);
    blockX = ( (a / ZNMapBlockLongitude4[level]) << 2 ) + (int)( ( a % ZNMapBlockLongitude4[level] ) * 4 / ZNMapBlockLongitude4[level]);

    int b = lat - MESH_BASE_COORY;
    //blockY = ( (b / ZNMapBlockLatitude4[level]) << 2 ) + ((int)(b * 4 / ZNMapBlockLatitude4[level]) & 0x03);
    blockY = ( (b / ZNMapBlockLatitude4[level]) << 2 ) + (int)( ( b % ZNMapBlockLatitude4[level] ) * 4 / ZNMapBlockLatitude4[level]);

    blockX += 4;
    blockY += 4;
}

void CMCMDNCoordTrans::ZNMapGetLLOfBlockRect( int level, int blockX, int blockY, int& minX, int& minY, int& maxX, int& maxY )
{
    blockX -= 4;
    blockY -= 4;

    minX = ((blockX>>2) * 1.0 * ZNMapBlockLongitude4[level]) +
        (((blockX&0x03) * 1.0 * ZNMapBlockLongitude4[level]) / 4) + MESH_BASE_COORX;
    blockX += 1;
    maxX = ((blockX>>2) * 1.0 * ZNMapBlockLongitude4[level]) +
        (((blockX&0x03) * 1.0 * ZNMapBlockLongitude4[level]) / 4) + MESH_BASE_COORX;

    minY = ((blockY>>2) * 1.0 * ZNMapBlockLatitude4[level]) +
        (((blockY&0x03) * 1.0 * ZNMapBlockLatitude4[level]) / 4) + MESH_BASE_COORY;
    blockY += 1;
    maxY = ((blockY>>2) * 1.0 * ZNMapBlockLatitude4[level]) +
        (((blockY&0x03) * 1.0 * ZNMapBlockLatitude4[level]) / 4) + MESH_BASE_COORY;
}

void CMCMDNCoordTrans::ZNMapGetBlockXYOfLL( int level, int blockX, int blockY, int lon, int lat, 
                                           int& x_block, int& y_block )
{
    int minX = 0;
    int minY = 0;
    int maxX = 0;
    int maxY = 0;
    CMCMDNCoordTrans::ZNMapGetLLOfBlockRect( level, blockX, blockY, minX, minY, maxX, maxY );

    // 基于 -128 ~ 127 基准
    x_block = (int)((lon-minX) * 255.0 / (ZNMapBlockLongitude4[level]/4.0)+0.5) - 128;
    y_block = (int)((lat-minY) * 255.0 / (ZNMapBlockLatitude4[level]/4.0)+0.5) - 128;
}

