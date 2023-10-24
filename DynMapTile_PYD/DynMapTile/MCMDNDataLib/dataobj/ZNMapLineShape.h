#pragma once
#include "ZNMapShape.h"
#include "ZNMapTypes.h"
#include "DataType.h"
#include "CPStl.h"


class ZNMapLineShape : public ZNMapShape
{
public:
    ZNMapLineShape(char kind, ZUINT32 blockX, ZUINT32 blockY):mBlockKind(kind), mBlockX(blockX), mBlockY(blockY)
    {
        mID = -1;
        mPointData1.array = NULL;
        mPointData1.num = 0;
    };
    ZNMapLineShape();
    ~ZNMapLineShape();


    ZUINT32 skipDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength);
	ZINT32 readDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength);

public:
    ZUINT32 mID;
    CP_VECTOR<ZNMapLine*> m_ZNMapLineAry;

protected:
	
    char mBlockKind;
    ZUINT32 mBlockX;
    ZUINT32 mBlockY;
    ZNInt8Section mPointData1;


};