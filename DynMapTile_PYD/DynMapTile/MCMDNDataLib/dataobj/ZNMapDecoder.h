#pragma once
//#include "ZNMapUtility.h"
#include "DataType.h"
#include <string>
#include "CPStl.h"
#include "ZNMapTile.h"
using namespace std;


typedef struct _tagSingleBlockDataInfo
{
	ZUINT32 ulBlcokDataLen;
	ZBYTE *pTileID;
	ZBYTE *pBlcokDataInfo;

}STSingleBlockDataInfo, *PSingleBlockDataInfo;


class ZNMapDecoder
{
public:
    static ZNMapDecoder& getInstance()
    {
        static ZNMapDecoder instance;
        return instance;
    }
    ~ZNMapDecoder();

	ZBOOL decodeBlockData(CP_VECTOR<STSingleBlockDataInfo*>* inputBlockDatas,
		CP_VECTOR<ZNMapTile*>* outputTiles);

protected:
    ZNMapDecoder();
    ZNMapDecoder(const ZNMapDecoder*);
    ZNMapDecoder & operator = (const ZNMapDecoder&);

private:
	CP_VECTOR<ZNMapTile*> mTiles;


};