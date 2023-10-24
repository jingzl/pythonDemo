#pragma once

//
//  ZNMapPolygonShape.h
//  ZDCNavi
//
//  Created by Wang.YingNa on 13/3/19.
//  Copyright 2008 SONORAN BLUE. All rights reserved.
//
/*
    ZNMapPolygonShape.h
    
    ZNMapPolygonShape
    ZNMapPolygonShape1
    ZNMapPolygonShape2
    ZNMapPolygonShape3
    ZNMapPolygonShape4
    ZNMapPolygonShape5
    ZNMapPolygonShape6
*/

#include "ZNMapShape.h"
#include "ZNMapTypes.h"
#include "CPStl.h"
#include "DataType.h"

class ZNMapPolygonShape : public ZNMapShape
{
public:
	ZINT32 skipDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength);
	ZINT32 readDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength);
protected:
	ZUINT32 mBlockX;
	ZUINT32 mBlockY;
	char mBlockKind;
public:
	ZNMapPolygonShape();
	~ZNMapPolygonShape();
};

class ZNMapPolygonShape1:public ZNMapPolygonShape
{
public:
	ZINT32 skipDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength);
	ZINT32 readDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength);
public:
	ZNMapPolygonShape1(char kind, ZUINT32 x, ZUINT32 y)
    {
        mBlockKind = kind;
        mBlockX = x;
        mBlockY = y;

		mPointList.array = NULL;
		mPointList.num = 0;
		mPolygonPoint.num = 0;
		mPolygonPoint.array = NULL;
		mLLPointList.array = NULL;
		mLLPointList.num = 0;
		mSeparatorDataPtr = NULL;
		mPolygonInfoPtr = NULL;
	};
	~ZNMapPolygonShape1();

protected:
	ZNInt8Section mPointList;
	ZNInt8Section mPolygonPoint;

public:
	ZNFloatSection mLLPointList;
	CP_VECTOR<ZNFloatSection*> mLLPolygonPoint;
    ZUINT8                 mSeparatorCount;
    ZNMapSeparatorInfo      *mSeparatorDataPtr;
    ZNMapPolygonInfo        *mPolygonInfoPtr;
	ZUINT16                mPolygonCount;
	
};


class ZNMapPolygonShape2:public ZNMapPolygonShape
{
public:
	ZINT32 skipDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength);
	ZINT32 readDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength);
protected:
	ZNInt8Section mPointList;
	ZNInt8Section mPolygonPoint;

public:
	ZNMapPolygonShape2(char kind, ZUINT32 x, ZUINT32 y)
	{
        mBlockKind = kind;
        mBlockX = x;
        mBlockY = y;

		mPointList.array = NULL;
		mPointList.num = 0;
		mPolygonPoint.num = 0;
		mPolygonPoint.array = NULL;
		mLLPointList.array = NULL;
		mLLPointList.num = 0;
		mSeparatorDataPtr = NULL;
		mPolygonInfoPtr = NULL;
	};
	~ZNMapPolygonShape2();
public:
	ZNFloatSection mLLPointList;
	CP_VECTOR<ZNFloatSection*> mLLPolygonPoint;
    ZUINT8                 mSeparatorCount;
    ZNMapSeparatorInfo      *mSeparatorDataPtr;
    ZUINT16                mPolygonCount;
    ZNMapPolygonInfo        *mPolygonInfoPtr;
};


class ZNMapPolygonShape3:public ZNMapPolygonShape
{
public:
	ZINT32 skipDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength);
	ZINT32 readDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength);
protected:
	ZNInt8Section mPointList;
	ZNInt8Section mPolygonPoint;
	ZUINT8              mPenUpCount;
	ZUINT8              *mPenUpListPtr;
public:
	ZNMapPolygonShape3(char kind, ZUINT32 blockX, ZUINT32 blockY)
	{
        mBlockKind = kind;
        mBlockX = blockX;
        mBlockY = blockY;

		mLLPointList.array = NULL;
		mPointList.array = NULL;
		mPolygonPoint.array = NULL;
		mLLPointList.num = 0;
		mPolygonPoint.num = 0;
		mPointList.num = 0;
		mSeparatorDataPtr = NULL;
		mPolygonInfoPtr = NULL;
		mPenUpListPtr = NULL;
	};
	~ZNMapPolygonShape3();
public:
	ZNFloatSection mLLPointList;
	CP_VECTOR<ZNFloatSection*> mLLPolygonPoint;
    ZUINT8                 mSeparatorCount;
    ZNMapSeparatorInfo      *mSeparatorDataPtr;
    ZUINT16                mPolygonCount;
    ZNMapPolygonInfo        *mPolygonInfoPtr;
	CP_VECTOR<ZNFloatSection*> mPenUpList;
};


class ZNMapPolygonShape4:public ZNMapPolygonShape
{
public:
	ZINT32 skipDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength);
	ZINT32 readDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength);
protected:
	ZNInt8Section mPolygonPoint;
public:
	ZNMapPolygonShape4(char kind, ZUINT32 blockX, ZUINT32 blockY)
	{
        mBlockKind = kind;
        mBlockX = blockX;
        mBlockY = blockY;

		mSeparatorDataPtr = NULL;
		mPolygonInfoPtr = NULL;
		mPolygonPoint.array = NULL;
		mPolygonPoint.num = 0;
	};
	~ZNMapPolygonShape4();
public:
    ZUINT8                 mSeparatorCount;
    ZNMapSeparatorInfo      *mSeparatorDataPtr;
    ZUINT16                mPolygonCount;
    ZNMapPolygonInfo        *mPolygonInfoPtr;
	CP_VECTOR<ZNFloatSection*> mLLPolygonPoint;
};


class ZNMapPolygonShape5:public ZNMapPolygonShape
{
public:
	ZINT32 skipDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength);
	ZINT32 readDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength);
protected:
	ZNInt8Section mPolygonPoint;
	ZNInt8Section mPointList;
public:
	ZNMapPolygonShape5(char kind, ZUINT32 blockX, ZUINT32 blockY)
	{
        mBlockKind = kind;
        mBlockX = blockX;
        mBlockY = blockY;

		mLLPointList.array = NULL;
		mPointList.array = NULL;
		mPolygonPoint.array = NULL;
		mLLPointList.num = 0;
		mPolygonPoint.num = 0;
		mPointList.num = 0;
		mSeparatorDataPtr = NULL;
		mPolygonInfoPtr = NULL;
	};
	~ZNMapPolygonShape5();
public:
	ZNFloatSection mLLPointList;
    ZUINT8                 mSeparatorCount;
    ZNMapSeparatorInfo      *mSeparatorDataPtr;
    ZUINT16                mPolygonCount;
    ZNMapPolygonInfo        *mPolygonInfoPtr;
	CP_VECTOR<ZNFloatSection*> mLLPolygonPoint;
};


class ZNMapPolygonShape6:public ZNMapPolygonShape
{
public:
	ZINT32 skipDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength);
	ZINT32 readDataBytes(const ZUINT8* dataPtr, ZUINT32 dataLength);
protected:
	ZNInt8Section mPolygonPoint;
	ZNInt8Section mPointList;
	ZUINT8                 mPenUpCount;
	ZUINT8                 *mPenUpListPtr;
	ZNMapSeparatorInfo      *mSeparatorDataPtr;
public:
	ZNMapPolygonShape6(char kind, ZUINT32 blockX, ZUINT32 blockY)
	{
        mBlockKind = kind;
        mBlockX = blockX;
        mBlockY = blockY;

		mLLPointList.array = NULL;
		mPointList.array = NULL;
		mPolygonPoint.array = NULL;
		mLLPointList.num = 0;
		mPolygonPoint.num = 0;
		mPointList.num = 0;
		mSeparatorDataPtr = NULL;
		mPolygonInfoPtr = NULL;
		mPenUpListPtr = NULL;
	};
	~ZNMapPolygonShape6();
public:
	ZNFloatSection mLLPointList;
    ZUINT8                 mSeparatorCount;
    ZUINT16                mPolygonCount;
    ZNMapPolygonInfo        *mPolygonInfoPtr;
	CP_VECTOR<ZNFloatSection*> mPenUpList;
	CP_VECTOR<ZNFloatSection*> mLLPolygonPoint;
};

extern ZINT32
ZNMapPolygonReadPenUpFromDataBytes(
    const ZUINT8               *inBytes,
    ZUINT32                    inLength,
    ZUINT8                     *outCount,
    ZUINT8                     **outDataPtr
);

extern ZINT32
ZNMapPolygonReadSeparatorFromDataBytes(
    const ZUINT8*              inBytes,
    ZUINT32                    inLength,
    ZUINT8                     *outCount,
    ZNMapSeparatorInfo          **outDataPtr
);

extern ZBOOL
ZNMapPolygonSplitPolygon(
    ZUINT8                     inPointCount,
    const ZINT8                *inPointPtr,
    ZUINT8                     *outSeparatorCount,
    ZNMapSeparatorInfo          **outSeparatorInfoPtr);

////edited by Copy 20110309 for convert point to 3D
extern void 
ConvertPointTo3D(
				 ZFLOAT32                       *InThePointListPtr,
				 ZFLOAT32                       *OutThePointFor3D,
				 ZUINT8		            	DR_PointCount);

extern ZINT32
ZNMapPolygonReadPointFromDataBytes(
								   const ZUINT8 *     inBytes,
								   ZUINT32            inLength,
								   ZNInt8Section *outPoint
								);


extern ZBOOL 
ZNMapPolygonCreateSeparatedPolygon( 
									ZUINT8                     inPointCount,
									const ZINT8            *inPointPtr,
									ZUINT8                     inSeparatorCount,
									const ZNMapSeparatorInfo    *inSeparatorDataPtr,
									ZUINT16                    *outPolygonCount,
									ZNMapPolygonInfo            **outPolygonInfoPtr,
									ZNInt8Section					*outPolygonPoint);

extern ZBOOL
ZNMapPolygonCreateSeparatedPolygon8(
									  ZUINT8                     inPointCount,
									  const ZINT8                *inPointPtr,
									  ZUINT8                     inSeparatorCount,
									  const ZNMapSeparatorInfo    *inSeparatorDataPtr,
									  ZUINT16                    *outPolygonCount,
									  ZNMapPolygonInfo            **outPolygonInfoPtr,
									  ZNInt8Section				*outPolygonPoint
									  );

extern ZVOID convertStandBlockXY2LL(const ZNInt8Section* srcPoint,
                                    ZNFloatSection *dstPoint,
                                    char kind, 
                                    ZUINT32 blockX,
                                    ZUINT32 blockY);