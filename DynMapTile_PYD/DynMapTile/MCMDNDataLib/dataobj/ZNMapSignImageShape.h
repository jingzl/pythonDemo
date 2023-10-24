//
//  ZNMapSignImageShape.h
//  TaiwanNavi
//
//  Created by Satoshi Muraki on 10/04/16.
#include  "ZNMapSignShape.h"
#include "DataType.h"
#include "ZNTextureTypes.h"




class ZNMapSignImageShape: ZNMapSignShape
{
public:
    ZNMapSignImageShape();
    ZNMapSignImageShape(char kind, ZUINT32 blockX, ZUINT32 blockY):mBlockKind(kind), mBlockX(blockX),mBlockY(blockY)
    {
        mSignData.mID = -1;
        mSignData.mNumber = -1;
    };
    ~ZNMapSignImageShape();

public:
	ZBOOL decode();
	ZINT32 readDataBytes1(const ZUINT8* dataBytes, ZUINT32 dataLength);
	ZUINT16  convCodeToSID(ZUINT16 kindCode);

public:
	ZNFloatPoint mLLCenterOffset;
    ZNMapSignData mSignData;

protected:
    char mBlockKind;
    ZUINT32 mBlockX;
    ZUINT32 mBlockY;
    ZNPoint mCenterOffset;

};

