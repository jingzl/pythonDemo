#include "ZNMapUtility.h"
#include "QuickFunc.h"
#include "ZNMapTypes.h"

#define MESH_BASE_COORX     (-648000000)
#define MESH_BASE_COORY     (-324000000)


extern ZBOOL
ZNMapTileGetBlockInfo( char* tileID, char *outKind, ZUINT32 *outX, ZUINT32 *outY)
{
	ZINT32 retval;
	retval = sscanf(tileID, "%c%u.%u", outKind, outX, outY);
	return (retval == 3);
}

extern ZVOID removeIndexesInRange(CP_VECTOR<ZUINT32>& indexSet,  ZUINT32 location, ZUINT32 length)
{
	do 
	{
		break_if(indexSet.size() == 0);
		sort(indexSet.begin(), indexSet.end());
		ZUINT32 firstToken = 0;
		ZUINT32 endToken = 0;
		ZBOOL bDidFound = ZFALSE;
		for (ZUINT32 i = 0; i < indexSet.size(); i++)
		{
			if (!bDidFound && *(indexSet.begin() + i) >= location)
			{
				firstToken = endToken = i;
				bDidFound = ZTRUE;
				continue;
			}
			if ( *(indexSet.begin() + i) >= location &&  *(indexSet.begin() + i) <= location + length - 1)
			{
				endToken = i;
				continue;
			}
		}
		if (bDidFound)
		{
			if (firstToken == endToken)
			{
				indexSet.erase(indexSet.begin() + firstToken);
			}else{
				indexSet.erase(indexSet.begin() + firstToken, indexSet.begin() + endToken + 1);//这个函数会删到endToken + 1前一位，所以为了将endToken删掉，必须加1
			}
		}
 	} while (0);
}

extern ZUINT8
ZNMapLevelFromKind(char kind)
{
	return (kind == kZNMapKind_g ? kZNMapLevel_g :
		kind == kZNMapKind_i ? kZNMapLevel_i :
		kind == kZNMapKind_j ? kZNMapLevel_j :
		kind == kZNMapKind_k ? kZNMapLevel_k :
		kind == kZNMapKind_l ? kZNMapLevel_l :
		kind == kZNMapKind_m ? kZNMapLevel_m :
		kind == kZNMapKind_n ? kZNMapLevel_n :
		kind == kZNMapKind_o ? kZNMapLevel_o : 
        kind == kZNMapKind_p ? kZNMapLevel_p : 
        kind == kZNMapKind_q ? kZNMapLevel_q : 0xff);
}

// 根据BlockXY获得经纬度范围
extern ZVOID ZNMapGetLLOfBlockRect( ZINT32 level, ZINT32 blockX, ZINT32 blockY, ZINT32& minX, ZINT32& minY, ZINT32& maxX, ZINT32& maxY )
{
    blockX -= 4;
    blockY -= 4;

	minX = ((blockX>>2) * 1.0 * ZNMapBlockLongitude[level]) +
		(((blockX&0x03) * 1.0 * ZNMapBlockLongitude[level]) / 4) + MESH_BASE_COORX;
	blockX += 1;
	maxX = ((blockX>>2) * 1.0 * ZNMapBlockLongitude[level]) +
		(((blockX&0x03) * 1.0 * ZNMapBlockLongitude[level]) / 4) + MESH_BASE_COORX;

	minY = ((blockY>>2) * 1.0 * ZNMapBlockLatitude[level]) +
		(((blockY&0x03) * 1.0 * ZNMapBlockLatitude[level]) / 4) + MESH_BASE_COORY;
	blockY += 1;
	maxY = ((blockY>>2) * 1.0 * ZNMapBlockLatitude[level]) +
		(((blockY&0x03) * 1.0 * ZNMapBlockLatitude[level]) / 4) + MESH_BASE_COORY;

}