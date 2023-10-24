#pragma once
#include <string>
#include "DataType.h"
#include "CPStl.h"

using namespace std;

extern ZBOOL
ZNMapTileGetBlockInfo(char* tileID, char *outKind, ZUINT32 *outX, ZUINT32 *outY);

extern ZVOID removeIndexesInRange(CP_VECTOR<ZUINT32>& indexSet,  ZUINT32 location, ZUINT32 length);

extern ZUINT8 ZNMapLevelFromKind(char kind);

extern ZVOID ZNMapGetLLOfBlockRect( int level, int blockX, int blockY, int& minX, int& minY, int& maxX, int& maxY );